#include "bastien.h"
#include <sys/wait.h>

void	execute_pipe(t_ast *ast, t_env **env)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!ast || ast->type != PIPE || !ast->left || !ast->right)
		return;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	// Fork pour exécuter la commande gauche du pipe
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		execute_ast(ast->left, env);
		exit(EXIT_SUCCESS);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		execute_ast(ast->right, env);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(-1, NULL, 0);
	waitpid(-1, NULL, 0);
}
