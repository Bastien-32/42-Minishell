#include "minishell.h"

int	return_error_restore_fds(t_all *all, int stdin_tmp, int stdout_tmp)
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

void	restore_fds(int stdin_tmp, int stdout_tmp)
{
	dup2(stdin_tmp, STDIN_FILENO);
	dup2(stdout_tmp, STDOUT_FILENO);
	close(stdin_tmp);
	close(stdout_tmp);
}

void	wait_all_children(t_all *all, pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		sig;

	pid = wait(&status);
	while (pid > 0)
	{
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				write(1, "Quit (core dumped)\n", 19);
			else if (sig == SIGINT)
				write(2, "\n", 1);
			all->exit_status = 128 + sig;
		}
		else if (WIFEXITED(status))
		{
			if (pid == last_pid)
				all->exit_status = WEXITSTATUS(status);
		}
		pid = wait(&status);
	}
}
