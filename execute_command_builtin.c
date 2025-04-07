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

int	execute_builtin(t_ast *ast, t_env *env)
{
	if (ft_strcmp(ast->cmd[0], "echo") == 0)
		return (echo_builtin(ast->cmd));
	 else if (ft_strcmp(ast->cmd[0], "cd") == 0)
		return (cd_builtin(ast->cmd, env));
	else if (ft_strcmp(ast->cmd[0], "pwd") == 0)
		return (pwd_builtin());
	else if (ft_strcmp(ast->cmd[0], "export") == 0)
		return (export_builtin(ast->cmd, env));
	/*else if (ft_strcmp(ast->cmd[0], "unset") == 0)
		return (unset_builtin(ast->cmd));
	else if (ft_strcmp(ast->cmd[0], "env") == 0)
		return (env_cmd(ast->cmd));
	else
		return (exit_cmd(ast->cmd)); */
	return (0);
}
