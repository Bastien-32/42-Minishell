#include "minishell.h"

int	handle_redirection(t_token **tokens, t_ast **current_cmd, t_all *all)
{
	t_token	*redir;

	redir = *tokens;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != COMMAND)
	{
		all->exit_status = 2;
		ft_putstr_fd("bash: syntax error near unexpected token ", 2);
		ft_putstr_fd("'newline'\n", 2);
		return (0);
	}
	if (!*current_cmd)
	{
		*current_cmd = new_ast_node(NULL);
		if (!*current_cmd)
		{
			all->exit_status = 1;
			return (0);
		}
		add_back_ast(all, *current_cmd, *tokens);
	}
	fill_redirection(*current_cmd, redir, (*tokens)->value);
	*tokens = (*tokens)->next;
	return (1);
}

int	handle_command(t_token **tokens, t_ast **current_cmd, t_all *all)
{
	char	**args;

	if (!get_args_and_count(tokens, current_cmd, all, &args))
		return (0);
	if (!*current_cmd)
	{
		*current_cmd = new_ast_node(args);
		if (!*current_cmd)
			return (free_cmd_args(args, -1), 0);
		if (!add_back_ast(all, *current_cmd, *tokens))
		{
			free_ast_error(*current_cmd);
			*current_cmd = NULL;
			return (0);
		}
	}
	else
	{
		free_cmd_args((*current_cmd)->cmd, -1);
		(*current_cmd)->cmd = args;
	}
	return (1);
}

void	fill_redirection(t_ast *ast, t_token *redir, char *filename)
{
	int	fd;

	if (redir->type == REDIR_IN || redir->type == HEREDOC)
	{
		free(ast->redir_in);
		ast->redir_in = ft_strdup(filename);
		ast->type_in = redir->type;
	}
	if (redir->type == REDIR_OUT || redir->type == APPEND)
	{
		if (ast->redir_out)
			free(ast->redir_out);
		ast->redir_out = ft_strdup(filename);
		ast->type_out = redir->type;
		if (redir->type == REDIR_OUT)
			fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else
			fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			perror("fill_redirection: open failed");
		else
			close(fd);
	}
}

int	pipe_in_first_position(t_token *tokens, t_all *all)
{
	if (tokens->type == PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		free_token_list(tokens);
		all->exit_status = 2;
		return (0);
	}
	return (1);
}
