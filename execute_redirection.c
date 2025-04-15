#include "bastien.h"

int	perror_message(char *str)
{
	g_exit_status = 1;
	perror(str);
	return (1);
}

int	ft_redir_in(t_ast *ast)
{
	int	fd;

	fd = open(ast->redir_in, O_RDONLY);
	if (fd < 0)
		return (perror_message("Error opening file REDIR_IN"));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (perror_message("Error dup2 REDIR_IN"));
	}
	close(fd);
	return (0);
}

int	ft_redir_out(t_ast *ast)
{
	int	fd;

	fd = open(ast->redir_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror_message("Error opening file REDIR_OUT"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror_message("Error dup2 REDIR_OUT"));
	}
	close(fd);
	return (0);
}

int	ft_append(t_ast *ast)
{
	int	fd;

	fd = open(ast->redir_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror_message("Error opening file APPEND"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (perror_message("Error dup2 APPEND"));
	}
	close(fd);
	return (0);
}

int	ft_heredoc(t_ast *ast)
{
	int		fd_tmp;
	char	*line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, ast->redir_in) == 0)
		{
			free(line);
			break ;
		}
		fd_tmp = open("heredoc_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd_tmp < 0)
		{
			free(line);
			return (perror_message("Error opening heredoc file"));
		}
		close(fd_tmp);
		free(line);
	}
	return (0);
}

int	execute_redirection(t_ast *ast)
{
	if (!ast)
		return (perror_message("Redirection: filename missing or invalid"));
	//printf(" [executing redirection] type = %d, file = %s\n", ast->type, ast->filename);
	if (ast->redir_in)
	{
		if (ast->type_in == REDIR_IN)
			return (ft_redir_in(ast));
		else if (ast->type_in == HEREDOC)
			return (ft_heredoc(ast));
	}
	if (ast->redir_out)
	{
		if (ast->type_out == REDIR_OUT)
			return (ft_redir_out(ast));
		else if (ast->type_out == APPEND)
			return (ft_append(ast));
	}
	return (1);
}
