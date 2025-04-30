#include "bastien.h"

int	env_builtin(t_ast *node, t_all *all)
{
	t_env	*tmp_env;

	tmp_env = all->env;
	if (node->cmd[1])
	{
		write(2, "env: too many arguments\n", 25);
		all->exit_status = 1;
		return (1);
	}
	while (tmp_env)
	{
		ft_printf("%s=%s\n", tmp_env->env_keyname, tmp_env->value);
		tmp_env = tmp_env->next;
	}
	return (0);
}
