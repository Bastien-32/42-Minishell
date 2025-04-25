#include "bastien.h"

/* int	echo_builtin(char **args)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	g_exit_status = 0;
	return (0);
} */

// tester (lignes 35 et 37) ne prendre que le premier -n  :
//		 modif premier if en while 
int	echo_builtin(char **args)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	while (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	g_exit_status = 0;
	return (0);
}
