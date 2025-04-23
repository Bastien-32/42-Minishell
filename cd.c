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

int	update_env_var(t_env *env, const char *key, char *new_value)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->env_keyname, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(new_value);
			if (!tmp->value)
				return (0);
			return (1);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	change_oldpwd(t_env *env)
{
	char	*old_pwd;

	old_pwd = get_env_value(env, "PWD");
	if (!old_pwd)
	{
		g_exit_status = 1;
		return (0);
	}
	if (!update_env_var(env, "OLDPWD", old_pwd))
	{
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

int	change_pwd(t_env *env)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		g_exit_status = 1;
		return (0);
	}
	if (!update_env_var(env, "PWD", new_pwd))
	{
		free(new_pwd);
		g_exit_status = 1;
		return (0);
	}
	free(new_pwd);
	return (1);
}

char	*ft_dir_to_reach(t_env *env, char *cmd)
{
	char	*target;

	if (!cmd)
	{
		target = get_env_value(env, "HOME");
		if (!target)
		{
			write(2, "cd: HOME not set\n", 18);
			g_exit_status = 1;
			return (NULL);
		}
	}
	else
		target = cmd;
	return (target);
}

int	cd_builtin(char **cmd, t_env *env)
{
	char	*dir_to_reach;

	dir_to_reach = ft_dir_to_reach(env, cmd[1]);
	if(!dir_to_reach)
		return (1);
	if(!change_oldpwd(env))
		return (1);
	if (chdir(dir_to_reach) != 0)
	{
		perror("cd");
		g_exit_status = 1;
		return (1);
	}
	if(!change_pwd(env))
		return (1);
	g_exit_status = 0;
	return (g_exit_status);
}
