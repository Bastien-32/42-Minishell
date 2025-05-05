#include "bastien.h"

sig_atomic_t	g_sigint_received = 0;
sig_atomic_t	g_exit_status = 0;
/* 
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
		setup_signals_parent();
		rl_catch_signals = 0;
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
		
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			free(line);
			continue;
		}
		
		
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
	free(all);
	return (0);
} */

void	free_tab(char **tab)
{
	int	i = 0;
	if (!tab)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}


int	main(int argc, char **argv, char **envp)
{
	t_all	*all;
	char	*line;
	char	*tmp_line;
	char	**lines;
	int		i;

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
		setup_signals_parent();
		rl_catch_signals = 0;
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
		
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			free(line);
			continue;
		}
		
		
		lines = ft_split(line, '\n');
		i = 0;
		while (lines[i])
		{
			all->ast = ft_build_tree(lines[i], all);
			if (all->ast)
			{
				execute_ast(all);
				free_ast_error(all->ast);
			}
			i++;
		}
		free_tab(lines);
		free(line);
	}
	free_env_list(all->env);
	free(all);
	return (0);
}
