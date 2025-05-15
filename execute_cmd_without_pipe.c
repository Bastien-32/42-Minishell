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

int	execute_external( t_ast *node, t_all *all)
{
	char	*cmd_path;
	char	**envp;
	pid_t	pid;

	if (!prepare_env_and_path(all, node, &cmd_path, &envp))
		return (0);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		free_tab(envp);
		all->exit_status = 1;
		return (0);
	}
	if (pid == 0)
		exec_child_process(cmd_path, all->ast, envp);
	wait_child_status(all, pid);
	free(cmd_path);
	free_tab(envp);
	return (0);
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

