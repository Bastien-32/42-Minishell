#include "bastien.h"

// Fonction pour afficher ttes les variables d environnt
void	print_env(t_env *env)
{
    t_env   *current;
    current = env;
	while (current)
	{
		ft_printf("%s=%s\n", current->env_keyname, current->value);
		    current = current->next;
	}
}

int	export_builtin(char **args, t_env *env)
{
	char	*dir;

	if (args[1] == NULL)
	{
        //imprimer la liste des variables d environnements
		print_env(env);
        // dir = get_env_value(env, "HOME");
		// if (dir == NULL)
		// {
		// 	ft_printf ("cd: HOME not set\n");
		// 	return (1);
		// }
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
