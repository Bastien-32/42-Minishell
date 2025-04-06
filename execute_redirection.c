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

	fd = open(ast->filename, O_RDONLY);
	if (fd < 0)
		return(perror_message("Error opening file REDIR_IN"));
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return(perror_message("Error dup2 REDIR_IN"));
	}
	close(fd);
	return (0);
}

int	ft_redir_out(t_ast *ast)
{
	int	fd;

	fd = open(ast->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	return(perror_message("Error opening file REDIR_OUT"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return(perror_message("Error dup2 REDIR_OUT"));
	}
	close(fd);
	return (0);
}

int	ft_append(t_ast *ast)
{
	int	fd;

	fd = open(ast->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return(perror_message("Error opening file APPEND"));
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return(perror_message("Error dup2 APPEND"));
	}
	close(fd);
	return (0);
}

int	execute_redirection(t_ast *ast, t_env **env)
{
	int	fd;

	if (ast->type == REDIR_IN)
		return (ft_redir_in(ast));
	else if (ast->type == REDIR_OUT)
		return (ft_redir_out(ast));
	else if (ast->type == APPEND)
		return (ft_append(ast));
	else if (ast->type == HEREDOC)
		return (ft_heredoc(ast));
	/* else
		return (1); */
}
