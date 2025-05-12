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

int	execute_command(t_ast *node, t_all *all)
{
	int	status;

	if (!node || !node->cmd || !node->cmd[0])
		return (1);
	if (!is_valid_command(node->cmd))
		return (0);
	if (node_builtin(node->cmd[0]))
		status = execute_builtin(node, all);
	else
		status = execute_external(node, all);
	if (status != 0)
		return (0);
	return (1);
}

int	execute_single(t_ast *node, t_all *all)
{
	//printf("on entre dans execute single \n");
	if (all->ast->redir_in || all->ast->redir_out)
	{
		if (!execute_redirection(node, all))
			return (0);
	}
	if (!all->ast->cmd || !all->ast->cmd[0])
		return (1);
	if (!execute_command(node, all))
		return (0);
	return (1);
}

void	execute_command_child(t_ast *node, t_all *all)
{
	char	*cmd_path;
	char	**envp;

	setup_signals_child();
	if (node->redir_in || node->redir_out)
	{
		if (!execute_redirection(node, all))
			exit(1);
	}
	if (!is_valid_command(node->cmd))
		exit(0);
	if (node_builtin(node->cmd[0]))
		exit(execute_builtin(node, all));
	else
	{
		if (!prepare_env_and_path(all, node, &cmd_path, &envp))
			exit(all->exit_status);
		execve(cmd_path, node->cmd, envp);
		perror("execve");
		free(cmd_path);
		free_array_envp(envp);
		exit(127);
	}
}

int	pipe_child_process(t_ast *node, t_all *all, int fd_in, int fd_out)
{
	if (fd_in != STDIN_FILENO)
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
	execute_command_child(node, all);
	exit(all->exit_status);
}

int	return_error_restore_fds(t_all *all,int stdin_tmp, int stdout_tmp)
{
	if (dup2(stdin_tmp, STDIN_FILENO) == -1)
		perror("restore stdin");
	if (dup2(stdout_tmp, STDOUT_FILENO) == -1)
		perror("restore stdout");
	close(stdin_tmp);
	close(stdout_tmp);
	if (all->exit_status == 0)
		all->exit_status = 127;
	return (0);
}

void	restore_std_and_wait_all_children(t_all *all ,pid_t last_pid,
	int stdin_tmp, int stdout_tmp)
{
	int		status;
	pid_t	pid;
	int		sig;

	dup2(stdin_tmp, STDIN_FILENO);
	dup2(stdout_tmp, STDOUT_FILENO);
	close(stdin_tmp);
	close(stdout_tmp);
	while ((pid = wait(&status)) > 0)
	{
		// if (pid == last_pid)
		// {
		// 	if (WIFEXITED(status))
		// 		g_exit_status = WEXITSTATUS(status);
		// 	else if (WIFSIGNALED(status))
		// 	{
		// 		sig = WTERMSIG(status);
		// 		if (sig == SIGQUIT)
		// 			write(2, "Quit (core dumped)\n", 19);
		// 		//else if (sig == SIGINT)
		// 		//	write(2, "\n", 1);
		// 		g_exit_status = 128 + sig;
		// 	}
		// }
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(1, "Quit (core dumped)\n", 19);
			else if (sig == SIGINT)
			{
				write(2, "\n", 1);
			}
			all->exit_status = 128 + sig;
		}
		else if (WIFEXITED(status))
		{
			if (pid == last_pid)
				all->exit_status = WEXITSTATUS(status);
		}
	}
}

int	execute_ast(t_all *all)
{
	int		fd_in;
	int		tmp_stdin; 
	int		tmp_stdout;
	pid_t	last_pid;
	t_ast	*temp;

	last_pid = -1;
	fd_in = 0;
	tmp_stdin = dup(STDIN_FILENO);
	tmp_stdout = dup(STDOUT_FILENO);
	temp = all->ast;
	while (all->ast)
	{
		if (all->ast->pipe_out == 0)
		{
			if (!execute_single(all->ast, all))
			{
				free_ast_error(temp);
				return (return_error_restore_fds(all, tmp_stdin, tmp_stdout));
			}
		}
		else
		{
			if (!execute_pipe(&all->ast, all, &fd_in, &last_pid))
			{
				free_ast_error(temp);
				return (return_error_restore_fds(all, tmp_stdin, tmp_stdout));
			}
		}
		all->ast = all->ast->next;
	}
	restore_std_and_wait_all_children(all, last_pid, tmp_stdin, tmp_stdout);
	free_heredocs_temp(temp);
	free_ast_error(temp);
	return (0);
}

void	execute_cmd_followed_by_pipe(t_ast *node, t_all *all, int *fd_in, int *pipe_fd)
{
	setup_signals_child();
	if (*fd_in != STDIN_FILENO && dup2(*fd_in, STDIN_FILENO) == -1)
		exit(perror_message(all, "dup2 fd_in failed"));
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		exit(perror_message(all, "dup2 pipe write failed"));
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	if ((node->redir_in || node->redir_out)
		&& !execute_redirection(node, all))
		exit(1);
	execute_command_child(node, all);
	exit(all->exit_status);
}

//void execute_last_cmd(t_all *all, t_ast *node, int *fd_in)
void execute_last_cmd(t_all *all, t_ast *node, int *fd_in)
{
	setup_signals_child();
	if (*fd_in != STDIN_FILENO && dup2(*fd_in, STDIN_FILENO) == -1)
		exit(perror_message(all, "dup2 last in failed"));
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	if (!execute_redirection(node, all))
		exit(1);
	execute_command_child(node, all);
	exit(all->exit_status);
}

int cmd_followed_by_pipe(t_all *all, t_ast **node, int *fd_in, int *pipe_fd)
{
	pid_t	pid; 

	if (pipe(pipe_fd) == -1)
		return (perror_message(all, "pipe failed"));
	pid = fork();
	if (pid == -1)
		return (perror_message(all, "fork failed"));
	if (pid == 0)
		execute_cmd_followed_by_pipe(*node, all, fd_in, pipe_fd);
	close(pipe_fd[1]);
	if (*fd_in != STDIN_FILENO)
		close(*fd_in);
	*fd_in = pipe_fd[0];
	*node = (*node)->next;
	return (1);
}

int	execute_pipe(t_ast **ast_ptr, t_all *all, int *fd_in, pid_t *last_pid)
{
	int		pipe_fd[2];
	pid_t	pid;
	t_ast	*node;

	node = *ast_ptr;
	while (node && node->pipe_out == 1)
		if (!cmd_followed_by_pipe(all, &node, fd_in, pipe_fd))
			return (0);
	// Dernier nœud du bloc de pipes
	if (node)
	{
		pid = fork();
		if (pid == -1)
			return (perror_message(all, "fork failed"));
		if (pid == 0)
			execute_last_cmd(all, node, fd_in);
		*last_pid = pid;
		if (*fd_in != STDIN_FILENO)
			close(*fd_in);
	}
	*ast_ptr = node;
	return (1);
}
