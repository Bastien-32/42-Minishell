#include "bastien.h"

int	env_builtin(char **args, t_env *env)
{
	t_env *tmp_env;
	tmp_env = env;
	if (args[1])
	{
		write(2, "env: too many arguments\n", 25);
		return (2);
	}
	while (tmp_env)
	{
		printf("%s=%s\n", tmp_env->env_keyname, tmp_env->value);
		tmp_env = tmp_env->next;
	}
	return (0);
}