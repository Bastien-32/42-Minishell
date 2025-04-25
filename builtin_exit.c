#include "bastien.h"

#include <readline/readline.h>
#include <readline/history.h>

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

static void	free_env_and_ast_in_builtin_exit(t_env *env, t_ast *ast)
{
	free_env_list(env);
	free_ast_error(ast);
	rl_clear_history();
}

int	exit_builtin(char **args, t_env *env, t_ast *ast)
{
	long	code;

	if (ft_is_numeric(args[1]) && args[2])
	{
		ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("bash : exit: too many arguments\n", 2);
		g_exit_status = 1;
		return (1);
	}
	ft_putstr_fd("exit\n", 2);
	if (!args[1])
	{
		free_env_and_ast_in_builtin_exit(env, ast);
		exit(0);
	}
	if (!ft_is_numeric(args[1]))
	{
		print_msg_exit_not_numeric(args[1]);
		free_env_and_ast_in_builtin_exit(env, ast);
		exit(2);
	}
	code = (unsigned char)ft_atol(args[1]);
	free_env_and_ast_in_builtin_exit(env, ast);
	exit(code);
}
