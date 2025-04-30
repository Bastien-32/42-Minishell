#include "bastien.h"

void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		ft_printf("%s=%s\n", current->env_keyname, current->value);
		current = current->next;
	}
}

int is_valid_identifier(const char *str)
{
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

int validate_and_split_env_var(t_all *all ,char *arg,
	char **env_keyname, char **value)
{
	if (!is_valid_identifier(arg))
	{
		ft_printf("export: '%s' is not a valid identifier\n", arg);
		all->exit_status = 1;
		return (0);
	}
	*env_keyname = arg;
	*value = strchr(arg, '=');
	if (*value != NULL)
	{
		**value = '\0';
		(*value)++;
	}
	else
	{
		*value = NULL;
	}
	return (1);
}

int	update_or_add_env_var(t_env *env, char *env_keyname, char *value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->env_keyname, env_keyname) == 0)
		{
			free(current->value);
			if (value != NULL)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	add_new_env_var(t_env **env, char *env_keyname, char *value)
{
	t_env	*new_env;

	if (value != NULL)
		new_env = create_env_node(env_keyname, value);
	else
		new_env = create_env_node(env_keyname, NULL);
	add_env_back(env, new_env);
}

int	export_builtin(t_all *all)
{
	int		i;
	char	*env_keyname;
	char	*value;

	if (all->ast->cmd[1] == NULL)
	{
		print_env(all->env);
		return (0);
	}
	i = 1;
	while (all->ast->cmd[i])
	{
		if (validate_and_split_env_var(all, all->ast->cmd[i], &env_keyname, &value))
		{
			if (value == NULL)
				return (0);
			else
			{
				if (!update_or_add_env_var(all->env, env_keyname, value))
				{
					add_new_env_var(&all->env, env_keyname, value);
				}
			}
		}
		i++;
	}
	return (0);
}

int	export_sorted(t_env *env)
{
	t_env	*current;
	int		count;
	char	**env_var;

	current = env;
	count = 0;
	while(current != NULL)
	{
		count++;
		current = current->next;
	}
	env_var = malloc(sizeof(char) * (count));
	if (!env_var)
		return (0);
	return (0);
}