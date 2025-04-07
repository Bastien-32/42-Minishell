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

int	validate_and_split_env_var(char *arg, char **env_keyname, char **value)
{
	*value = strchr(arg, '=');
	if (*value == NULL)
	{
		ft_printf("export: '%s' is not a valid identifier\n", arg);
		return (0);
	}
	*env_keyname = arg;
	*(*value) = '\0';
	(*value)++;
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
			current->value = ft_strdup(value);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	add_new_env_var(t_env **env, char *env_keyname, char *value)
{
	t_env	*new_env;

	new_env = create_env_node(env_keyname, value);
	add_env_back(env, new_env);
}

int	export_builtin(char **args, t_env **env)
{
	int		i;
	char	*env_keyname;
	char	*value;

	if (args[1] == NULL)
	{
		print_env(*env);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (validate_and_split_env_var(args[i], &env_keyname, &value))
		{
			if (!update_or_add_env_var(env, env_keyname, value))
			{
				add_new_env_var(env, env_keyname, value);
			}
		}
		i++;
	}
	return (0);
}
