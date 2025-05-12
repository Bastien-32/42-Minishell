#include "bastien.h"

volatile sig_atomic_t	g_sigint_received = 0;
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

void	handle_ctrl_d_in_heredoc(char *line, t_ast *node, t_all *all)
{
	int	fd_tmp;

	write(1, "\033[1A", 4);     // remonter d'une ligne
	write(1, "\033[2K", 4);     // effacer la ligne
	write(1, "heredoc>\n ", 10);   // réafficher l'invite
	ft_putstr_fd("bash: warning: « here-document » delimited ", 1);
	ft_putstr_fd("by end-of-file (wanted « ", 1);
	ft_putstr_fd(node->redir_in, 1);
	ft_putstr_fd(" »)\n", 1);
	fd_tmp = open("heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_tmp >= 0)
		close(fd_tmp);
	all->exit_status = 0;
	free(line);
}

void	ft_heredoc(t_ast *node, t_all *all, int hd_index)
{
	int		fd_tmp;
	char	*line;
	char	*heredoc_file;
	char	*index_str;

	index_str = ft_itoa(hd_index);
	heredoc_file = ft_strjoin("heredoc_tmp", index_str);
	free(index_str);
	fd_tmp = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	close(fd_tmp);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			handle_ctrl_d_in_heredoc(line, node, all);
			break ;
		}
		if (ft_strcmp(line, node->redir_in) == 0)
		{
			free(line);
			break ;
		}
		fd_tmp = open(heredoc_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		/* if (fd_tmp < 0)
			return (perror_message(all, "open heredoc_file")); */
		write(fd_tmp, line, ft_strlen(line));
		write(fd_tmp, "\n", 1);
		close(fd_tmp);
		free(line);
	}
	free(node->redir_in);
	node->redir_in = ft_strdup(heredoc_file);
	node->type_in = REDIR_IN;
	free(heredoc_file);
}

void	prepare_heredocs(t_all *all)
{
	t_ast	*node = all->ast;
	int	i;

	i = 0;
	while (node)
	{
		i++;
		if (node->type_in == HEREDOC)
			ft_heredoc(node, all, i);
		node = node->next;
	}
}

void free_heredocs_temp(t_ast *ast)
{
	t_ast	*node = ast;

	while (node)
	{
		if (node->redir_in && ft_strncmp("heredoc_tmp", node->redir_in, 11) == 0)
			unlink(node->redir_in);
		node = node->next;
	}
}
int	main(int argc, char **argv, char **envp)
{
	t_all	*all;
	char	*line;
	char	*tmp_line;
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
		if (g_sigint_received)
		{
			g_sigint_received = 0;
			all->exit_status = 0;
			continue;
		}
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
		{
			free(line);
			free_env_list(all->env);
			free(all);
			return (write(1, "exit\n", 5), 0);
		}
		
		
		if (line[0] == '\0')
		{
			free(line);
			continue;
		}

		if (*line && isatty(STDIN_FILENO))
			add_history(line);
		
		all->lines = ft_split(line, '\n');
		i = 0;
		while (all->lines[i])
		{
			all->ast = ft_build_tree(all->lines[i], all);
			//prepare_heredocs(all);
			if (all->ast)
			{
				prepare_heredocs(all);
				execute_ast(all);
			}
			all->ast = NULL;
			i++;
		}
		free_tab(all->lines);
		free(line);
	}
	free_env_list(all->env);
	free(all);
	return (0);
}
