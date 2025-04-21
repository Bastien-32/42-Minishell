#include "bastien.h"

int	perror_message(char *str)
{
	g_exit_status = 1;
	perror(str);
	return (0);
}

int	ft_redir_in(t_ast *ast)
{
	int	fd;

	//printf("[REDIR_IN] read from: %s\n", ast->redir_out);

	fd = open(ast->redir_in, O_RDONLY);
	if (fd < 0)
		return (perror_message("Error opening file REDIR_IN"));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (perror_message("Error dup2 REDIR_IN"));
	}
	close(fd);
	return (1);
}

int	ft_redir_out(t_ast *ast)
{
	int	fd;

	//printf("[REDIR_OUT] redirecting to: %s\n", ast->redir_out);

	fd = open(ast->redir_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror_message("Error opening file REDIR_OUT"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror_message("Error dup2 REDIR_OUT"));
	}
	close(fd);
	return (1);
}

int	ft_append(t_ast *ast)
{
	int	fd;

	//printf("[APPEND] redirecting to: %s\n", ast->redir_out);
	fd = open(ast->redir_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror_message("Error opening file APPEND"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror_message("Error dup2 APPEND"));
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

void	handle_ctrl_d(char *line)
{
	int	fd_tmp;

	write(1, "\033[1A", 4);     // remonter d'une ligne
	write(1, "\033[2K", 4);     // effacer la ligne
	write(1, "heredoc> ", 9);   // réafficher l'invite
	fd_tmp = open("heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_tmp >= 0)
		close(fd_tmp);
	g_exit_status = 0;
	free(line);
}

int	put_heredoc_in_stdin(void)
{
	int	fd_read;

	if (access("heredoc_tmp", F_OK) == -1)
	{
		//printf("on sort\n");
		g_exit_status = 0;
		return (1);
	}
	fd_read = open("heredoc_tmp", O_RDONLY);
	if (fd_read < 0)
		return (perror_message("open heredoc_tmp (read)"));
	if (dup2(fd_read, STDIN_FILENO) == -1)
	{
		close(fd_read);
		return (perror_message("dup2 heredoc"));
	}
	close(fd_read);
	unlink("heredoc_tmp");
	g_exit_status = 0;
	return (1);
}

int	ft_heredoc(t_ast *ast)
{
	int		fd_tmp;
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			handle_ctrl_d(line);
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
	return (put_heredoc_in_stdin());
}

int	execute_redirection(t_ast *ast)
{
	int	status;

	if (!ast)
		return (perror_message("Redirection: filename missing or invalid"));
	status = 1;
	if (ast->redir_in)
	{
		//printf("execute REDIR_IN\n");
		if (ast->type_in == REDIR_IN)
			status = ft_redir_in(ast);
		else if (ast->type_in == HEREDOC)
			status = ft_heredoc(ast);
	}
	if (ast->redir_out)
	{
		//printf("execute REDIR_OUT\n");
		if (ast->type_out == REDIR_OUT)
			status = ft_redir_out(ast);
		else if (ast->type_out == APPEND)
			status = ft_append(ast);
	}
	if (status != 1)
		return (0);
	return (1);
}
