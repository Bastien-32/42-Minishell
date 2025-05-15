#include "bastien.h"

int	is_valid_identifier(const char *str)
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

int	validate_and_split_env_var(t_all *all, char *arg,
	char **env_keyname, char **value)
{
	char	*equal_sign;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("bash: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		all->exit_status = 1;
		return (0);
	}
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign != NULL)
	{
		*equal_sign = '\0';
		*env_keyname = arg;
		*value = equal_sign + 1;
	}
	else
	{
		*env_keyname = arg;
		*value = NULL;
	}
	return (1);
}

int	update_or_add_env_var(t_env *env, char *env_keyname, char *value)
{
	t_env	*lst;

	lst = env;
	while (lst)
	{
		if (ft_strcmp(lst->env_keyname, env_keyname) == 0)
		{
			if (value == NULL && lst->value != NULL)
				return (1);
			free(lst->value);
			if (value != NULL)
				lst->value = ft_strdup(value);
			else
				lst->value = NULL;
			return (1);
		}
		lst = lst->next;
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

int	export_builtin(t_ast *node, t_all *all)
{
	int		i;
	char	*env_keyname;
	char	*value;

	if (node->cmd[1] == NULL)
		return (export_sorted(all->env), 0);
	i = 1;
	while (node->cmd[i])
	{
		if (validate_and_split_env_var(all, node->cmd[i], &env_keyname, &value))
		{
			if (value == NULL)
			{
				if (!update_or_add_env_var(all->env, env_keyname, NULL))
					add_new_env_var(&all->env, env_keyname, NULL);
			}
			else
			{
				if (!update_or_add_env_var(all->env, env_keyname, value))
					add_new_env_var(&all->env, env_keyname, value);
			}
		}
		i++;
	}
	return (0);
}
