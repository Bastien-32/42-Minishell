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

char	*actualize_pwd(t_env **tmp_env,char *name_dir_to_reach)
{
	
}

char	*change_dir(t_env *env, char *name_dir_to_reach)
{
	char	*actual_path_dir_value;
	char	*new_path_dir;
	t_env	*tmp_env;

	actual_path_dir_value = get_env_value(env, "PWD");
	tmp_env = env;
	while (tmp_env)
	{
		if (ft_strcmp(tmp_env->env_keyname, "PWD"))
		{
			tmp_env->value = actualise_pwd(&tmp_env, name_dir_to_reach);
		}
		if (ft_strcmp(tmp_env->env_keyname, "OLDPWD"))
		{
			tmp_env->value = actual_path_dir_value;
			break ;
		}
		tmp_env = tmp_env->next;
	}
	return (name_dir_to_reach);
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
			g_exit_status = 1;
			return (1);
		}
	}
	else
		dir = args[1];
	if (chdir(dir) != 0)
	{
		perror ("cd");
		g_exit_status = 1;
		return (1);
	}
	g_exit_status = 0;
	return (0);
}
