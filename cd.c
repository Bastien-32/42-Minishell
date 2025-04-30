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

int	change_oldpwd(t_all *all)
{
	char	*old_pwd;

	old_pwd = get_env_value(all->env, "PWD");
	if (!old_pwd)
	{
		g_exit_status = 1;
		return (0);
	}
	if (!update_env_var(all->env, "OLDPWD", old_pwd))
	{
		all->exit_status = 1;
		return (0);
	}
	return (1);
}

int	change_pwd(t_all *all)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		g_exit_status = 1;
		return (0);
	}
	if (!update_env_var(all->env, "PWD", new_pwd))
	{
		free(new_pwd);
		all->exit_status = 1;
		return (0);
	}
	free(new_pwd);
	return (1);
}

char	*ft_dir_to_reach(t_all *all, char *cmd)
{
	char	*target;

	if (!cmd)
	{
		target = get_env_value(all->env, "HOME");
		if (!target)
		{
			write(2, "cd: HOME not set\n", 18);
			all->exit_status = 1;
			return (NULL);
		}
	}
	else
		target = cmd;
	return (target);
}

int	cd_builtin(t_all *all)
{
	char	*dir_to_reach;

	dir_to_reach = ft_dir_to_reach(all, all->ast->cmd[1]);
	if(!dir_to_reach)
		return (1);
	if(!change_oldpwd(all))
		return (1);
	if (chdir(dir_to_reach) != 0)
	{
		perror("cd");
		all->exit_status = 1;
		return (1);
	}
	if(!change_pwd(all))
		return (1);
	all->exit_status = 0;
	return (g_exit_status);
}
