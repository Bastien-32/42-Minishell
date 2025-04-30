#include "bastien.h"

int	echo_builtin(t_all *all)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	while (all->ast->cmd[i] && ft_strcmp(all->ast->cmd[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (all->ast->cmd[i])
	{
		write(1, all->ast->cmd[i], strlen(all->ast->cmd[i]));
		if (all->ast->cmd[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	all->exit_status = 0;
	return (0);
}
