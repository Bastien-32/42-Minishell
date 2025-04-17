#include "bastien.h"

int	is_valid_command(char **cmd)
{
	int	i;

	if (!cmd || !cmd[0])
		return (0);
	if (cmd[0][0] == '\0')
		return (0);
	i = 0;
	while (cmd[0][i])
	{
		if (cmd[0][i] != ' ' && cmd[0][i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

int	execute_command(t_ast *ast, t_env **env)
{
	int	status;

	if (!ast || !ast->cmd || !ast->cmd[0])
		return (1);
	if (!is_valid_command(ast->cmd))
		return (0);
	if (node_builtin(ast->cmd[0]))
		status = execute_builtin(ast, env);
	else
		status = execute_external(ast, *env);
	if (status != 0)
		return (0);
	return (1);
}

void	restore_std_fds(int stdin_tmp, int stdout_tmp)
{
	dup2(stdin_tmp, STDIN_FILENO);
	dup2(stdout_tmp, STDOUT_FILENO);
	close(stdin_tmp);
	close(stdout_tmp);
}

int	execute_single(t_ast *ast, t_env **env)
{
	printf("on entre dans execute single \n");
	if (ast->redir_in || ast->redir_out)
	{
		if (!execute_redirection(ast))
			return (0);
	}
	if (!ast->cmd || !ast->cmd[0])
		return (1);
	if (!execute_command(ast, env))
		return (0);
	return (1);
}

void	execute_command_child(t_ast *ast, t_env *env)
{
	if (!is_valid_command(ast->cmd))
		exit(0);
	if (node_builtin(ast->cmd[0]))
		exit(execute_builtin(ast, &env));
	else
		exit(execute_external(ast, env));
}

int	pipe_child_process(t_ast *ast, t_env *env, int fd_in, int fd_out)
{
	if (fd_in != 0)
	{
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			exit(1);
		}
		close(fd_in);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		exit(1);
	}
	close(fd_out);
	if (fd_in != 0 && fd_in != STDIN_FILENO)
		close(fd_in);
	if (fd_out != STDOUT_FILENO)
		close(fd_out);
	for (int i = 3; i < 1024; i++)
		close(i);
	if (ast->redir_in || ast->redir_out)
	{
		if (!execute_redirection(ast))
			exit(1);
	}
	execute_command_child(ast, env);
	exit(g_exit_status);
}

/* int	execute_pipe(t_ast *ast, t_env **env, int *fd_in)
{
	int			pipe_fd[2];
	pid_t		pid;

	printf("[PIPE] lancement pipe: %s\n", ast->cmd[0]);
	if (pipe(pipe_fd) == -1)
		return (perror_message("create pipe failed"));
	pid = fork();
	if (pid == -1)
		return (perror_message("create fork pipe failed"));
	if (pid == 0)
	{
		printf("on entre dans le process enfant\n");
		if (!pipe_child_process(ast, *env, *fd_in, pipe_fd[1]))
			exit(g_exit_status);
	}
	close(pipe_fd[1]);
	if (*fd_in != 0)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	return (1);
} */


int	return_error_restore_fds(int stdin_tmp, int stdout_tmp)
{
	if (dup2(stdin_tmp, STDIN_FILENO) == -1)
		perror("restore stdin");
	if (dup2(stdout_tmp, STDOUT_FILENO) == -1)
		perror("restore stdout");
	close(stdin_tmp);
	close(stdout_tmp);
	return (1);
}

void	wait_all_children(void)
{
	int	status;
	pid_t	pid;

	while ((pid = wait(&status)) > 0)
	{
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
}
int	execute_ast(t_ast *ast, t_env **env)
{
	int	fd_in;
	int	tmp_stdin; 
	int	tmp_stdout;

	fd_in = 0;
	tmp_stdin = dup(STDIN_FILENO);
	tmp_stdout = dup(STDOUT_FILENO);
	while (ast)
	{
		printf("[EXEC] cmd: %s | pipe_out: %d\n", ast->cmd ? ast->cmd[0] : "(null)", ast->pipe_out);
		if (ast->pipe_out == 0)
		{
			if (!execute_single(ast, env))
				return (return_error_restore_fds(tmp_stdin, tmp_stdout));
		}
		else
		{
			if (!execute_pipe(&ast, env, &fd_in))
				return (return_error_restore_fds(tmp_stdin, tmp_stdout));
		}
		ast = ast->next;
	}
	restore_std_fds(tmp_stdin, tmp_stdout);
	wait_all_children();
	return (0);
}

int	execute_pipe(t_ast **ast_ptr, t_env **env, int *fd_in)
{
	int		pipe_fd[2];
	pid_t	pid;
	t_ast	*node;

	node = *ast_ptr;
	while (node && node->pipe_out == 1)
	{
		if (pipe(pipe_fd) == -1)
			return (perror_message("pipe failed"));
		pid = fork();
		if (pid == -1)
			return (perror_message("fork failed"));
		if (pid == 0)
		{
			if (*fd_in != STDIN_FILENO)
			{
				if (dup2(*fd_in, STDIN_FILENO) == -1)
					exit(perror_message("dup2 fd_in failed"));
				close(*fd_in);
			}
			if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
				exit(perror_message("dup2 pipe write failed"));
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			if (!execute_redirection(node))
				exit(1);
			execute_command_child(node, *env);
			exit(g_exit_status);
		}
		close(pipe_fd[1]);
		if (*fd_in != STDIN_FILENO)
			close(*fd_in);
		*fd_in = pipe_fd[0];
		node = node->next;
	}
	// Dernier nœud du bloc de pipes
	if (node)
	{
		pid = fork();
		if (pid == -1)
			return (perror_message("fork failed"));
		if (pid == 0)
		{
			if (*fd_in != STDIN_FILENO)
			{
				if (dup2(*fd_in, STDIN_FILENO) == -1)
					exit(perror_message("dup2 last in failed"));
				close(*fd_in);
			}
			if (!execute_redirection(node))
				exit(1);
			execute_command_child(node, *env);
			exit(g_exit_status);
		}
		if (*fd_in != STDIN_FILENO)
			close(*fd_in);
	}

	*ast_ptr = node;
	return (1);
}

