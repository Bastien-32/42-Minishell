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

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path_array = parse_path(env, "PATH");
	if (!path_array)
		return (NULL);
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
	return (free_array_envp(path_array), NULL);
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

int	prepare_env_and_path(t_all *all, t_ast *node, char **cmd_path, char ***envp)
{
	*cmd_path = find_path(node->cmd[0], all->env);
	if (!*cmd_path)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(node->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		all->exit_status = 127;
		return (0);
	}
	*envp = env_to_array(all->env);
	if (!*envp)
	{
		perror("env_to_array");
		free(*cmd_path);
		all->exit_status = 1;
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
	// signal(SIGINT, SIG_DFL);
	// signal(SIGQUIT, SIG_DFL);
	//printf("Child PID = %d\n", getpid());
	setup_signals_child();
	execve(cmd_path, ast->cmd, envp);
	perror("execve");
	exit(127);
}

void	wait_child_status(t_all *all, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		all->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		all->exit_status = 128 + WTERMSIG(status);
}

int	execute_external( t_ast *node, t_all *all)
{
	char	*cmd_path;
	char	**envp;
	pid_t	pid;

	if (!prepare_env_and_path(all, node, &cmd_path, &envp))
		return (1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		free_array_envp(envp);
		all->exit_status = 1;
		return (1);
	}
	if (pid == 0)
		exec_child_process(cmd_path, all->ast, envp);
	wait_child_status(all, pid);
	free(cmd_path);
	free_array_envp(envp);
	//printf("end execute_external\n");
	return (0);
}
