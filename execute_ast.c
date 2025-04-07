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
	if (node_builtin(ast->cmd[0]))
		execute_builtin(ast, env);
	else
		execute_external(ast, *env);
	//print_env(*env);										//a supprimer apres ainsi que la fonction
	return (g_exit_status);
}

// une fois toutes les redirections codées, il faudra les ordonner 
// comme la fonction en commentaire après celle-là.
int	execute_ast(t_ast *ast, t_env **env)
{
	while (ast)
	{
		if (ast->type == COMMAND)
			execute_command(ast, env);
		else if (ast->type == REDIR_IN || ast->type == REDIR_OUT
			|| ast->type == APPEND || ast->type == HEREDOC)
			execute_redirection(ast);
		/* else if (ast->type == PIPE)
			execute_pipe(ast, env); */
		ast = ast->right;
	}
	return (1);
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
