#include "bastien.h"

char	**parse_path(t_env *env, char *key)
{
	char	*path_value;
	char	**path_array;
	t_env	*temp_env;

	temp_env = env;
	while (temp_env)
	{
		if (ft_strcmp(temp_env->env_keyname, key) == 0)
			break ;
		temp_env = temp_env->next;
	}
	path_value = temp_env->value;
	path_array = ft_split(path_value, ':');
	if (!path_array)
		return (NULL);
	return (path_array);
}

char	*find_path(char *cmd, t_env *env)
{
	char	*folder;
	char	*cmd_path;
	char	**path_array;
	int		i;

	path_array = parse_path(env, "PATH");
	if (!path_array)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	i = 0;
	while (path_array[i])
	{
		folder = ft_strjoin(path_array[i], "/");
		if (!folder)
			return (free_array_envp(path_array), NULL);
		cmd_path = ft_strjoin_free_s1(folder, cmd);
		if (!cmd_path)
			return (free_array_envp(path_array), NULL);
		if (access(cmd_path, X_OK) == 0)
			return (free_array_envp(path_array), cmd_path);
		free(cmd_path);
		i++;
	}
	return (0);
}

void	free_array_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**env_to_array(t_env *env)
{
	char	**envp;
	int		i;

	i = env->nb_env;
	envp = malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (env)
	{
		envp[i] = ft_strjoin(env->env_keyname, "=");
		if (!envp[i])
			return (free_array_envp(envp), NULL);
		envp[i] = ft_strjoin_free_s1(envp[i], env->value);
		if (!envp[i])
			return (free_array_envp(envp), NULL);
		i++;
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}

int	prepare_env_and_path(t_ast *ast, t_env *env, char **cmd_path, char ***envp)
{
	*cmd_path = find_path(ast->cmd[0], env);
	if (!*cmd_path)
	{
		perror(ast->cmd[0]);
		g_exit_status = 127;
		return (0);
	}
	*envp = env_to_array(env);
	if (!*envp)
	{
		perror("env_to_array");
		free(*cmd_path);
		g_exit_status = 1;
		return (0);
	}
	return (1);
}

void	exec_child_process(char *cmd_path, t_ast *ast, char **envp)
{
	/*int	i;

	i = 0;
 	printf("[child] launching execve: %s\n", cmd_path);
	while (ast->cmd[i])
	{
		printf("cmd[%d] = %s\n", i, ast->cmd[i]);
		i++;
	} */
	execve(cmd_path, ast->cmd, envp);
	perror("execve");
	exit(127);
}

void	wait_child_status(int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
}

int	execute_external(t_ast *ast, t_env *env)
{
	char	*cmd_path;
	char	**envp;
	pid_t	pid;

	if (!prepare_env_and_path(ast, env, &cmd_path, &envp))
		return (1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		free_array_envp(envp);
		g_exit_status = 1;
		return (1);
	}
	if (pid == 0)
		exec_child_process(cmd_path, ast, envp);
	wait_child_status(pid);
	free(cmd_path);
	free_array_envp(envp);
	printf("end execute_external\n");
	return (0);
}
