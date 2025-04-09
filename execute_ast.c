#include "bastien.h"

//fonction a supprimer
/* void	print_env(t_env *env)
{
	while (env)
	{
		printf("env: %s=%s\n", env->env_keyname, env->value);
		env = env->next;
	}
} */

//lignes a supprimer dans fonction
int	execute_command(t_ast *ast, t_env **env)
{
	//printf("FD stdout: %d\n", STDOUT_FILENO);
	if (!ast || !ast->cmd || !ast->cmd[0])
		return (1);
	if (node_builtin(ast->cmd[0]))
		return(execute_builtin(ast, env));
	else
		return(execute_external(ast, *env));
	//print_env(*env);										//a supprimer apres ainsi que la fonction
	return (g_exit_status);
}

// utils_fd.c ou utils_exec.c, comme tu préfères
void	restore_std_fds(int stdin_tmp, int stdout_tmp)
{
	dup2(stdin_tmp, STDIN_FILENO);
	dup2(stdout_tmp, STDOUT_FILENO);
	close(stdin_tmp);
	close(stdout_tmp);
}

int	execute_ast(t_ast *ast, t_env **env)
{
	t_ast	*redir;
	int		stdin_tmp;
	int		stdout_tmp;

	while (ast)
	{
		if (ast->type == COMMAND)
		{
			stdout_tmp = dup(STDOUT_FILENO);
			stdin_tmp = dup(STDIN_FILENO);
			redir = ast->right;
			while (redir && (redir->type == REDIR_IN || redir->type == HEREDOC
					|| redir->type == APPEND || redir->type == REDIR_OUT))
			{
				if (execute_redirection(redir) != 0)
					return (1);
				redir = redir->right;
			}
			execute_command(ast, env);
			restore_std_fds(stdin_tmp, stdout_tmp);
			ast = redir;
		}
		else
			ast = ast->right;
	}
	return (0);
}
