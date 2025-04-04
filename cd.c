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

int	cd_builtin(char **args, t_env *env)
{
	char	*dir;

	if (args[1] == NULL)
	{
		dir = get_env_value(env, "HOME");
		if (dir == NULL)
		{
			ft_printf ("cd: HOME not set\n");
			return (1);
		}
	}
	else
		dir = args[1];
	if (chdir(dir) != 0)
	{
		perror ("cd");
		return (1);
	}
	return (0);
}
