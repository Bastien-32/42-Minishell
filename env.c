#include "bastien.h"

t_env	*create_env_node(const char *env_keyname, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->env_keyname = ft_strdup(env_keyname);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

void	add_env_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if(!new)
		free_env_error(*env, 1);
	if (!*env)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;
	char	*equal_pos;
	char	*env_keyname;
	char	*value;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			env_keyname = ft_strndup(envp[i], equal_pos - envp[i]);
			value = ft_strdup(equal_pos + 1);
			if (!env_keyname || !value)
				free_env_error(env, 1);
			add_env_back(&env, create_env_node(env_keyname, value));
			free(env_keyname);
			free(value);
		}
		i++;
	}
	return (env);
}

void	free_env_error(t_env *env, int need_exit)
{
	free_env_list(env);
	write(2, "Malloc node env failed", 23);
	if (need_exit)
		exit(1);
}

void	free_env_list(t_env *env)
{
	t_env *tmp;

	while (env)
	{
		tmp = env->next;
		free(env->env_keyname);
		free(env->value);
		free(env);
		env = tmp;
	}
}
