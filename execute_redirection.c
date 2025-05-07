#include "bastien.h"

int	perror_message(t_all *all, char *str)
{
	all->exit_status = 1;
	fflush(stdout);										// À supprimer à la fin
	perror(str);
	return (0);
}

int	ft_redir_in(t_ast *node, t_all *all)
{ 
	int	fd;

	fd = open(node->redir_in, O_RDONLY);
	if (fd < 0)
		return (perror_message(all, "Error opening file REDIR_IN"));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (perror_message(all, "Error dup2 REDIR_IN"));
	}
	close(fd);
	return (1);
}

int	ft_redir_out(t_ast *node, t_all *all)
{
	int	fd;

	fd = open(node->redir_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror_message(all, "Error opening file REDIR_OUT"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror_message(all, "Error dup2 REDIR_OUT"));
	}
	close(fd);
	return (1);
}

int	ft_append(t_ast *node, t_all *all)
{
	int	fd;

	fd = open(node->redir_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror_message(all, "Error opening file APPEND"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror_message(all, "Error dup2 APPEND"));
	}
	close(fd);
	return (1);
}

/* int	ft_heredoc(t_ast *ast)
{
	int		fd_tmp;
	int		fd_read;
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			write(1, "\033[1A", 4);      // remonter d'une ligne
			write(1, "\033[2K", 4);      // effacer toute la ligne
			write(1, "heredoc> ", 9);    // réécrire l'invite
			free(line);
			break;
		}
		if (ft_strcmp(line, ast->redir_in) == 0)
		{
			free(line);
			break ;
		}
		fd_tmp = open("heredoc_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd_tmp < 0)
			return (perror_message("open heredoc_tmp"));
		write(fd_tmp, line, ft_strlen(line));
		write(fd_tmp, "\n", 1);
		close(fd_tmp);
		free(line);
	}
	fd_read = open("heredoc_tmp", O_RDONLY);
	if (fd_read < 0)
		return (perror_message("open heredoc_tmp (read)"));
	if (dup2(fd_read, STDIN_FILENO) == -1)
		return (close(fd_read), perror_message("dup2 heredoc"));
	close(fd_read);
	unlink("heredoc_tmp");
	return (1);
} */

/* void	handle_ctrl_d(char *line, t_ast *node, t_all *all)
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
} */

int	put_heredoc_in_stdin(t_all *all)
{
	int	fd_read;

	if (access("heredoc_tmp", F_OK) == -1)
	{
		//printf("on sort\n");
		all->exit_status = 0;
		return (1);
	}
	fd_read = open("heredoc_tmp", O_RDONLY);
	if (fd_read < 0)
		return (perror_message(all, "open heredoc_tmp (read)"));
	if (dup2(fd_read, STDIN_FILENO) == -1)
	{
		close(fd_read);
		return (perror_message(all, "dup2 heredoc"));
	}
	close(fd_read);
	unlink("heredoc_tmp");
	all->exit_status = 0;
	return (1);
}

/* int	ft_heredoc(t_ast *node, t_all *all)
{
	int		fd_tmp;
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			handle_ctrl_d(line, node, all);
			break ;
		}
		if (ft_strcmp(line, node->redir_in) == 0)
		{
			free(line);
			break ;
		}
		fd_tmp = open("heredoc_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd_tmp < 0)
			return (perror_message(all, "open heredoc_tmp"));
		write(fd_tmp, line, ft_strlen(line));
		write(fd_tmp, "\n", 1);
		close(fd_tmp);
		free(line);
	}
	return (put_heredoc_in_stdin(all));
} */

int	execute_redirection(t_ast *node, t_all *all)
{
	int	status;

	/* if (!node->ast)
		return (perror_message(node, "Redirection: filename missing or invalid")); */
	status = 1;
	if (node->redir_in)
	{
		//printf("execute REDIR_IN\n");
		if (node->type_in == REDIR_IN)
			status = ft_redir_in(node, all);
		/* else if (node->type_in == HEREDOC)
			status = ft_heredoc(node, all); */
	}
	if (node->redir_out)
	{
		//printf("execute REDIR_OUT\n");
		if (node->type_out == REDIR_OUT)
			status = ft_redir_out(node, all);
		else if (node->type_out == APPEND)
			status = ft_append(node, all);
	}
	if (status != 1)
		return (0);
	return (1);
}
