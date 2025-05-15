#include "bastien.h"

void	free_tab(char **tab)
{
	int	i = 0;
	if (!tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

void	free_ast_error(t_ast *ast)
{
	int		i;
	t_ast	*next;

	if (ast == NULL)
		return ;
	while (ast)
	{
		next = ast->next;
		if (ast->cmd)
		{
			i = 0;
			while (ast->cmd[i])
				free(ast->cmd[i++]);
			free(ast->cmd);
		}
		if (ast->redir_in)
			free(ast->redir_in);
		if (ast->redir_out)
			free(ast->redir_out);
		free(ast);
		ast = next;
	}
}

void	free_env_error(t_env *env, int need_exit)
{
	free_env_list(env);
	if (need_exit)
	{
		write(2, "Malloc node env failed\n", 24);
		exit(1);
	}
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->env_keyname);
		if (env->value)
			free(env->value);
		free(env);
		env = tmp;
	}
}
