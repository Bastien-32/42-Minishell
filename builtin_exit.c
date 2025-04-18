#include "bastien.h"

#include <readline/readline.h>
#include <readline/history.h>

static int	ft_is_numeric(const char *str)
{
	int	i;

	i = 0;
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

static void	free_env_and_ast_in_builtin_exit(t_env *env, t_ast *ast)
{
	free_env_list(env);
	free_ast_error(ast);
	rl_clear_history();
}

int	exit_builtin(char **args, t_env *env, t_ast *ast)
{
	long	code;

	if (args[1] && args[2])
		return (ft_putstr_fd("Exit : too many arguments\n", 2), 1);
	ft_putstr_fd("exit\n", 2);
	if (!args[1])
	{
		free_env_and_ast_in_builtin_exit(env, ast);
		exit(0);
	}
	if (!ft_is_numeric(args[1]))
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(" : numeric operator is expected\n", 2);
		free_env_and_ast_in_builtin_exit(env, ast);
		exit(2);
	}
	code = (unsigned char)ft_atol(args[1]);
	free_env_and_ast_in_builtin_exit(env, ast);
	exit(code);
}
