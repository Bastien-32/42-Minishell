#include "bastien.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->env_keyname, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	swap_env_values(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_val;

	tmp_key = a->env_keyname;
	tmp_val = a->value;
	a->env_keyname = b->env_keyname;
	a->value = b->value;
	b->env_keyname = tmp_key;
	b->value = tmp_val;
}

void	sort_env_list(t_env *env)
{
	t_env	*lst;
	int		swapped;

	if (!env)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		lst = env;
		while (lst->next)
		{
			if (ft_strcmp(lst->env_keyname, lst->next->env_keyname) > 0)
			{
				swap_env_values(lst, lst->next);
				swapped = 1;
			}
			lst = lst->next;
		}
	}
}

int	export_sorted(t_env *env)
{
	t_env	*copy;
	t_env	*lst;

	copy = NULL;
	lst = env;
	while (lst)
	{
		add_env_back(&copy, create_env_node(lst->env_keyname, lst->value));
		lst = lst->next;
	}
	sort_env_list(copy);
	lst = copy;
	while (lst)
	{
		if (lst->value)
			ft_printf("declare -x %s=\"%s\"\n", lst->env_keyname, lst->value);
		else
			ft_printf("declare -x %s\n", lst->env_keyname);
		lst = lst->next;
	}
	free_env_list(copy);
	return (0);
}
