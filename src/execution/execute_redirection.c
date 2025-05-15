#include "minishell.h"

int	perror_message(t_all *all, char *str)
{
	all->exit_status = 1;
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

int	execute_redirection(t_ast *node, t_all *all)
{
	int	status;

	status = 1;
	if (node->redir_in)
	{
		if (node->type_in == REDIR_IN)
			status = ft_redir_in(node, all);
	}
	if (node->redir_out)
	{
		if (node->type_out == REDIR_OUT)
			status = ft_redir_out(node, all);
		else if (node->type_out == APPEND)
			status = ft_append(node, all);
	}
	if (status != 1)
		return (0);
	return (1);
}
