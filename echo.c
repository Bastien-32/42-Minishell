#include "bastien.h"

int	echo_builtin(t_ast *node, t_all *all)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	while (node->cmd[i] && ft_strcmp(node->cmd[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (node->cmd[i])
	{
		write(1, node->cmd[i], strlen(node->cmd[i]));
		if (node->cmd[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	all->exit_status = 0;
	return (0);
}
