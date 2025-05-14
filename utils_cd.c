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