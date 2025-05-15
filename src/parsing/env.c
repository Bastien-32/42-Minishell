#include "minishell.h"

t_env	*create_env_node(const char *env_keyname, const char *value)
{
	t_env	*node;

	node = ft_calloc(1, sizeof(t_env));
	if (!node)
		return (NULL);
	node->env_keyname = ft_strdup(env_keyname);
	if (value != NULL)
		node->value = ft_strdup(value);
	else
		node->next = NULL;
	node->next = NULL;
	return (node);
}

void	add_env_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!new)
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

void	create_shlvl(t_env **env)
{
	char	*env_keyname;
	char	*value;

	env_keyname = ft_strdup("SHLVL");
	value = ft_strdup("1");
	if (!env_keyname || !value)
		free_env_error(*env, 1);
	add_env_back(env, create_env_node(env_keyname, value));
	free(env_keyname);
	free(value);
}

void	handle_shlvl(t_env **env)
{
	t_env	*tmp_env;
	char	*new_shlvl;
	int		shlvl;

	tmp_env = *env;
	while (tmp_env)
	{
		if (ft_strcmp("SHLVL", tmp_env->env_keyname) == 0)
		{
			shlvl = ft_atoi(tmp_env->value);
			free(tmp_env->value);
			new_shlvl = ft_itoa(shlvl + 1);
			if (!new_shlvl)
				free_env_error(*env, 1);
			tmp_env->value = new_shlvl;
			return ;
		}
		tmp_env = tmp_env->next;
	}
	create_shlvl(env);
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
	handle_shlvl(&env);
	return (env);
}
