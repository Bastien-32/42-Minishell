#include "bastien.h"

sig_atomic_t	g_exit_status = 0;

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
				//ft_putnbr_fd(ft_strlen(line), 1);
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
		ast = build_tree(line, env);
		if (ast)
		{
			execute_ast(ast, &env);
			free_ast_error(ast);
			//printf("[main loop] back to prompt\n");							//a supprimer
		}
		else
			g_exit_status = 0;
		//free(line);
	}
	free_env_list(env);
	return (0);
}
