#include "minishell.h"

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
	if (!execute_ast_loop(all, temp, &fd_in, &last_pid))
		return (return_error_restore_fds(all, tmp_stdin, tmp_stdout));
	restore_fds(tmp_stdin, tmp_stdout);
	wait_all_children(all, last_pid);
	free_heredocs_temp(temp);
	free_ast_error(temp);
	return (0);
}

int	execute_ast_loop(t_all *all, t_ast *start, int *fd_in, pid_t *last_pid)
{
	while (all->ast)
	{
		if (all->ast->pipe_out == 0)
		{
			if (!execute_cmd_without_pipe(all->ast, all))
			{
				free_ast_error(start);
				return (0);
			}
		}
		else
		{
			if (!execute_cmd_with_pipe(&all->ast, all, fd_in, last_pid))
			{
				free_ast_error(start);
				return (0);
			}
		}
		all->ast = all->ast->next;
	}
	return (1);
}

int	execute_cmd_without_pipe(t_ast *node, t_all *all)
{
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

int	execute_cmd_with_pipe(t_ast **ast_ptr, t_all *all,
	int *fd_in, pid_t *last_pid)
{
	int		pipe_fd[2];
	pid_t	pid;
	t_ast	*node;

	node = *ast_ptr;
	while (node && node->pipe_out == 1)
		if (!cmd_followed_by_pipe(all, &node, fd_in, pipe_fd))
			return (0);
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

void	execute_last_cmd(t_all *all, t_ast *node, int *fd_in)
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
