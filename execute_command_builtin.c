#include "bastien.h"

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

int	execute_builtin(t_ast *node, t_all *all)
{
	if (ft_strcmp(node->cmd[0], "echo") == 0)
		return (echo_builtin(node, all));
	else if (ft_strcmp(node->cmd[0], "cd") == 0)
		return (cd_builtin(node, all));
	else if (ft_strcmp(node->cmd[0], "pwd") == 0)
		return (pwd_builtin(all));
	else if (ft_strcmp(node->cmd[0], "export") == 0)
		return (export_builtin(node, all));
	else if (ft_strcmp(node->cmd[0], "env") == 0)
		return (env_builtin(node, all));
	else if (ft_strcmp(node->cmd[0], "unset") == 0)
		return (unset_builtin(node, all));
	else
		return (exit_builtin(node, all));
}
