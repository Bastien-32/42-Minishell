#include "minishell.h"

void	exec_child_process(char *cmd_path, t_ast *ast, char **envp)
{
	setup_signals_child();
	execve(cmd_path, ast->cmd, envp);
	perror("execve");
	exit(127);
}

void	wait_child_status(t_all *all, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		all->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		all->exit_status = 128 + WTERMSIG(status);
}
