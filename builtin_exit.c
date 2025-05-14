#include "bastien.h"

static int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[0] == '\0')
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

void	print_msg_exit_not_numeric(char *str)
{
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static void	free_env_and_ast_in_builtin_exit(t_all *all)
{
	free_env_list(all->env);
	free_ast_error(all->ast);
	free_tab(all->lines);
	free(all);
	rl_clear_history();
}

int	exit_builtin(t_ast *node, t_all *all)
{
	long	code;

	if (node->cmd[1] && ft_is_numeric(node->cmd[1]) && node->cmd[2])
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("bash : exit: too many arguments\n", 2);
		all->exit_status = 1;
		return (0);
	}
	ft_putstr_fd("exit\n", 2);
	if (!node->cmd[1])
	{
		free_env_and_ast_in_builtin_exit(all);
		exit(0);
	}
	if (!ft_is_numeric(node->cmd[1]))
	{
		print_msg_exit_not_numeric(node->cmd[1]);
		free_env_and_ast_in_builtin_exit(all);
		exit(2);
	}
	code = (unsigned char)ft_atol(node->cmd[1]);
	free_env_and_ast_in_builtin_exit(all);
	exit(code);
}
