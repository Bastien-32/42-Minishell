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

int	unset_builtin(char **args, t_env **env)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	remove_node_env(env, args, i);
	g_exit_status = 0;
	return (0);
}
