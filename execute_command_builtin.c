#include "bastien.h"

int	echo_builtin(char **args)
{
	int	n_flag;
	int	i;

	n_flag = 0;
	i = 1;
	if (args[1] && strcmp(args[1], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (n_flag)
		printf("%%");
	printf("\n");
	return (0);
}

int	node_builtin(char *name_cmd)
{
	if (ft_strcmp(name_cmd, "echo") == 0
		|| ft_strcmp(name_cmd, "cd") == 0
		|| ft_strcmp(name_cmd, "pwd") == 0
		|| ft_strcmp(name_cmd, "export") == 0
		|| ft_strcmp(name_cmd, "unset") == 0
		|| ft_strcmp(name_cmd, "env") == 0
		|| ft_strcmp(name_cmd, "exit") == 0)
				return (1);
	return (0);
}

int	execute_builtin(t_ast *ast)
{
	if (ft_strcmp(ast->cmd[0], "echo") == 0)
		return (echo_builtin(ast->cmd));
	/* else if (ft_strcmp(ast->cmd[0], "cd") == 0)
		return (cd_builtin(ast->cmd));
	else if (ft_strcmp(ast->cmd[0], "pwd") == 0)
		return (pwd_builtin(ast->cmd));
	else if (ft_strcmp(ast->cmd[0], "export") == 0)
		return (export_builtin(ast->cmd));
	else if (ft_strcmp(ast->cmd[0], "unset") == 0)
		return (unset_builtin(ast->cmd));
	else if (ft_strcmp(ast->cmd[0], "env") == 0)
		return (env_cmd(ast->cmd));
	else
		return (exit_cmd(ast->cmd)); */
	return (0);
}
