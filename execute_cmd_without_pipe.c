#include "bastien.h"

int	execute_command(t_ast *node, t_all *all)
{
	int	status;

	if (!node || !node->cmd || !node->cmd[0])
		return (1);
	if (!is_valid_command(node->cmd))
		return (0);
	if (node_builtin(node->cmd[0]))
		status = execute_builtin(node, all);
	else
		status = execute_external(node, all);
	if (status != 0)
		return (0);
	return (1);
}

int	is_valid_command(char **cmd)
{
	int	i;

	if (!cmd || !cmd[0])
		return (0);
	if (cmd[0][0] == '\0')
		return (0);
	i = 0;
	while (cmd[0][i])
	{
		if (cmd[0][i] != ' ' && cmd[0][i] != '\t')
			return (1); 
		i++;
	}
	return (0);
}

