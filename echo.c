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
	return (0);
}

void	execute_ast_command(t_ast *ast)
{
	if (ast && ast->type == COMMAND)
	{
		if (strcmp(ast->cmd[0], "echo") == 0)
		{
			echo_builtin(ast->cmd);
		}
		else if(strcmp(ast->cmd[0], "cd") == 0)
		{
			cd_builtin(ast->cmd);
		}
	}
}