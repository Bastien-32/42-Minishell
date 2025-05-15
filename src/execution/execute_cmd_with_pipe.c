#include "minishell.h"

int	cmd_followed_by_pipe(t_all *all, t_ast **node, int *fd_in, int *pipe_fd)
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

void	execute_cmd_followed_by_pipe(t_ast *node, t_all *all,
			int *fd_in, int *pipe_fd)
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

void	execute_command_child(t_ast *node, t_all *all)
{
	char	*cmd_path;
	char	**envp;
	int		exit_status;

	setup_signals_child();
	if (node->redir_in || node->redir_out)
	{
		if (!execute_redirection(node, all))
			exit(1);
	}
	if (!is_valid_command(node->cmd))
		exit(0);
	if (node_builtin(node->cmd[0]))
	{
		exit_status = execute_builtin(node, all);
		exit_child_builtin(all, exit_status);
	}
	if (!prepare_env_and_path(all, node, &cmd_path, &envp))
		exit(all->exit_status);
	execve(cmd_path, node->cmd, envp);
	perror("execve");
	free(cmd_path);
	free_tab(envp);
	exit(127);
}

void	exit_child_builtin(t_all *all, int exit_status)
{
	free_ast_error(all->ast);
	free_env_list(all->env);
	free_tab(all->lines);
	free(all);
	exit(exit_status);
}
