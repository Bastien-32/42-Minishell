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

// une fois toutes les redirections codées, il faudra les ordonner 
// comme la fonction en commentaire après celle-là.
/*int	execute_ast(t_ast *ast, t_env **env)
{
	while (ast)
	{
		if (ast->type == REDIR_IN || ast->type == REDIR_OUT
			|| ast->type == APPEND || ast->type == HEREDOC)
			execute_redirection(ast);
		if (ast->type == COMMAND)
			execute_command(ast, env);
		else if (ast->type == PIPE)
			execute_pipe(ast, env);
		ast = ast->right;
	}
	return (0);
}*/

int	execute_ast(t_ast *ast, t_env **env)
{
	t_ast	*redir;

	while (ast)
	{
		// Traitement des redirections liées à la commande
		if (ast->type == COMMAND)
		{
			redir = ast->right;

			while (redir && (redir->type == REDIR_IN || redir->type == HEREDOC
					|| redir->type == APPEND || redir->type == REDIR_OUT))
			{
				if (redir == ast || redir->right == redir)
				{
					write(2, "⚠️ Redirection circulaire détectée\n", 35);
					return (1);
				}
				if (execute_redirection(redir) != 0)
					return (1);
				if (redir->right == NULL)
					printf("-> End of redirections\n");
				else if (redir->right == redir)
				{
					write(2, "❗redirection right pointe sur elle-même (boucle)\n", 50);
					break;
				}

				redir = redir->right;
			}

			execute_command(ast, env);

			if (!redir)
				break;
			if (redir == NULL)
				printf("-> Next node is NULL\n");
			else
				printf("-> Next node is of type %d\n", redir->type);

			ast = redir;
		}
		else
		{
			// Si ce n’est pas une commande, on saute
			ast = ast->right;
		}
	}
	return (0);
}

// Il faudra à la fin exécuter cette fonction  dans ce sens-là.
//La fonction au-dessus permet juste de tester dans le sens où on crée les fonctions.
/* 
int	execute_ast(t_ast *ast, t_env **env)
{
	if (!ast)
		return (0);

	if (ast->type == PIPE)
		return (execute_pipe(ast, env));
	else if (ast->type == REDIR_IN || ast->type == REDIR_OUT || ast->type == APPEND)
		return (execute_redirection(ast, env));
	else if (ast->type == HEREDOC)
		return (execute_heredoc(ast, env));
	else if (ast->type == COMMAND)
		return (execute_command(ast, env));

	return (1); // Type inconnu : code d'erreur générique
} */
