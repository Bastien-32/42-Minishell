#include "bastien.h"

int	echo_builtin(char **args)
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
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!n_flag)
		ft_printf("\n");
	g_exit_status = 0;
	return (0);
}
