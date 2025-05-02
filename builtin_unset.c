#include "bastien.h"

void	remove_node_env(t_env **env, char **args, int i)
{
	t_env	*tmp_env;
	t_env	*prev_env;

	while (args[i])
	{
		tmp_env = *env;
		prev_env = NULL;
		while (tmp_env)
		{
			if (ft_strcmp(tmp_env->env_keyname, args[i]) == 0)
			{
				if (prev_env)
					prev_env->next = tmp_env->next;
				else
					*env = tmp_env->next;
				free(tmp_env->env_keyname);
				free(tmp_env->value);
				free(tmp_env);
				break ;
			}
			prev_env = tmp_env;
			tmp_env = tmp_env->next;
		}
		i++;
	}
}

int	unset_builtin(t_ast *node, t_all *all)
{
	int	i;

	if (!node->cmd[1])
		return (0);
	i = 1;
	remove_node_env(&all->env, node->cmd, i);
	all->exit_status = 0;
	return (0);
}
