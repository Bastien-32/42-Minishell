#include "bastien.h"

int	handle_redirection_in_command(t_token *tokens, t_ast **current_cmd,
	t_all *all)
{
	t_token	*redir;

	redir = tokens;
	tokens = tokens->next;
	if (!tokens || tokens->type == PIPE)
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
		add_back_ast(all, *current_cmd, tokens);
	}
	fill_redirection(*current_cmd, redir, tokens->value);
	return (1);
}

int	count_cmd_tokens(t_token *tokens, t_ast **current_cmd, t_all *all)
{
	int		count;
	t_token	*tmp_tokens;

	count = 0;
	tmp_tokens = tokens;
	while (tmp_tokens && (tmp_tokens->type == COMMAND
			|| tmp_tokens->type == REDIR_IN || tmp_tokens->type == APPEND
			|| tmp_tokens->type == REDIR_OUT || tmp_tokens->type == HEREDOC))
	{
		if (tmp_tokens->type == REDIR_IN || tmp_tokens->type == APPEND
			|| tmp_tokens->type == REDIR_OUT || tmp_tokens->type == HEREDOC)
		{
			if (!handle_redirection_in_command(tmp_tokens, current_cmd, all))
				return (-1);
			tmp_tokens = tmp_tokens->next->next;
			if (!tmp_tokens)
				return (count);
			continue ;
		}
		count++;
		tmp_tokens = tmp_tokens->next;
	}
	return (count);
}

char	**dup_cmd_tokens(t_token **tokens, int count, t_all *all)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		perror("Malloc failed");
		all->exit_status = 1;
		return (NULL);
	}
	i = 0;
	while (i < count && *tokens && (*tokens)->type != PIPE)
	{
		if ((*tokens)->type == REDIR_IN || (*tokens)->type == APPEND
			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == HEREDOC)
		{
			*tokens = (*tokens)->next->next;
			continue ;
		}
		args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}

void	free_cmd_args(char **args, int count)
{
	if (!args)
		return ;
	while (--count >= 0)
		free(args[count]);
	free(args);
}

int	get_args_and_count(t_token **tokens, t_ast **current_cmd,
	t_all *all, char ***args_out)
{
	int	count;

	count = count_cmd_tokens(*tokens, current_cmd, all);
	if (count < 0)
		return (0);
	*args_out = dup_cmd_tokens(tokens, count, all);
	if (!*args_out)
		return (0);
	return (1);
}
