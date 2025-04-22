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
	int	i;
	int	count_returns;

	i =0;
	count_returns = 0;
	while (name_dir_to_reach[i])
	{
		if (name_dir_to_reach[i] == '.' && name_dir_to_reach[i + 1] == '.' && name_dir_to_reach[i + 2] == '/')
		{
			count_returns++;
			i += 3;
		}
		else
			i++;
	}
	if (count_returns)
	{
		//compter depuis la fin le nombre de slash et s'arreter qand on en compte autant que count_returns
		//ensuite garder le chemin entre le debut de la phrase et ce slash
		// le copier dans un tmp_pwd
	}
	//si un nom apres le dernier de la commande cd, rajouter un / apres ce qu'on vient de copier dans tmp_pwd 
	//copier dans tmp_pwd le nom de ce fichier
	//verifier si ce chemin existe et se deplacer dedans (ca c'est surement ce que fais la fonction  cd_builtin apres)
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
			tmp_env->value = actualise_pwd(&tmp_env, name_dir_to_reach);
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
