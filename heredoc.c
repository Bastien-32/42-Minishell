#include "bastien.h"

void	handle_ctrl_d_in_heredoc(char *line, t_ast *node, t_all *all)
{
	int	fd_tmp;

	write(1, "\033[1A", 4);
	write(1, "\033[2K", 4);
	write(1, "heredoc>\n ", 10);
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

void	read_and_write_heredoc_lines(t_ast *node, t_all *all,
	const char *heredoc_file)
{
	int		fd_tmp;
	char	*line;

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
		write(fd_tmp, line, ft_strlen(line));
		write(fd_tmp, "\n", 1);
		close(fd_tmp);
		free(line);
	}
}

void	ft_heredoc(t_ast *node, t_all *all, int hd_index)
{
	int		fd_tmp;
	char	*index_str;
	char	*heredoc_file;

	index_str = ft_itoa(hd_index);
	heredoc_file = ft_strjoin("heredoc_tmp", index_str);
	free(index_str);
	fd_tmp = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	close(fd_tmp);
	read_and_write_heredoc_lines(node, all, heredoc_file);
	free(node->redir_in);
	node->redir_in = ft_strdup(heredoc_file);
	node->type_in = REDIR_IN;
	free(heredoc_file);
}

void	prepare_heredocs(t_all *all)
{
	t_ast	*node;
	int		i;

	i = 0;
	node = all->ast;
	while (node)
	{
		i++;
		if (node->type_in == HEREDOC)
			ft_heredoc(node, all, i);
		node = node->next;
	}
}

void	free_heredocs_temp(t_ast *ast)
{
	t_ast	*node;

	node = ast;
	while (node)
	{
		if (node->redir_in
			&& ft_strncmp("heredoc_tmp", node->redir_in, 11) == 0)
			unlink(node->redir_in);
		node = node->next;
	}
}
