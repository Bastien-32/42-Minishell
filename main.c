#include "bastien.h"

sig_atomic_t	g_exit_status = 0;
/* 
int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_ast	*ast;
	char	*line;
	char	*tmp_line;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	setup_signals_main();
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			line = readline("minishell> ");
			if (g_exit_status == 130)
			{
				if (line[0] == '\0')
				{
					rl_on_new_line();
					rl_redisplay();
				}
				g_exit_status = 0;
				free(line);
				continue;
			}
		}
		else
		{
			tmp_line = get_next_line(fileno(stdin));
			if (tmp_line)
			{
				line = ft_strtrim(tmp_line, "\n");
				free(tmp_line);
			}
			else
				line = NULL;
		}
		if (!line && !isatty(STDIN_FILENO))
		{
			g_exit_status = 0;
			break;
		}
		else if (!line && isatty(STDIN_FILENO))
		{
			write(1, "\033[1A", 4);
			write(1, "\033[2K", 4);
			write(1, "minishell> ", 12);
			ft_printf("exit\n");
			g_exit_status = 0;
			break ;
		}
		if (*line && isatty(STDIN_FILENO))
			add_history(line);
		ast = ft_build_tree(line, env);
		if (ast)
		{
			execute_ast(ast, &env);
			free_ast_error(ast);
		}
		//free(line);
	}
	free_env_list(env);
	return (0);
}
 */

int	main(int argc, char **argv, char **envp)
{
	t_all	*all;
	char	*line;
	char	*tmp_line;

	(void)argc;
	(void)argv;
	all = malloc(sizeof(t_all));
	if (!all)
		return (1);
	all->env = init_env(envp);
	all->exit_status = 0;
	all->ast = NULL;
	while (1)
	{
		//setup_signals_main();
		if (isatty(STDIN_FILENO))
			line = readline("minishell> ");
		else
		{
			tmp_line = get_next_line(fileno(stdin));
			if (tmp_line)
			{
				line = ft_strtrim(tmp_line, "\n");
				free(tmp_line);
			}
			else
				line = NULL;
		}
		if (!line && !isatty(STDIN_FILENO))
			break ;
		if (!line && isatty(STDIN_FILENO))
			return (write(1, "\033[1A\033[2Kminishell> exit\n", 25), 0);
		if (*line && isatty(STDIN_FILENO))
			add_history(line);
		all->ast = ft_build_tree(line, all);
		if (all->ast)
		{
			execute_ast(all);
			free_ast_error(all->ast);
		}
	}
	free_env_list(all->env);
	return (all->exit_status);
}
