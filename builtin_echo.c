#include "bastien.h"

int	not_n(char *str)
{
	int	i;

	i = 2;
	if (str[0] != '-')
		return (1);
	if (str[1] != 'n')
		return (1);
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	echo_builtin(t_ast *node, t_all *all)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	while (node->cmd[i])
	{
		if (not_n(node->cmd[i]))
			break ;
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
