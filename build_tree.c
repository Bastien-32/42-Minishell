#include "bastien.h"

//fonction a supprimer
void	print_ast(t_ast *ast)
{
	int	i;

	printf("abstract syntax tree :\n");
	while (ast)
	{
		printf("─── New command ───\n");
		if (ast->cmd)
		{
			i = 0;
			while (ast->cmd[i])
			{
				printf("  CMD[%d] : %s\n", i, ast->cmd[i]);
				i++;
			}
		}
		if (ast->redir_in)
		{
			if (ast->type_in == REDIR_IN)
				printf("  REDIR_IN  < : %s\n", ast->redir_in);
			else if (ast->type_in == HEREDOC)
				printf("  HEREDOC   << : %s\n", ast->redir_in);
		}
		if (ast->redir_out)
		{
			if (ast->type_out == REDIR_OUT)
				printf("  REDIR_OUT > : %s\n", ast->redir_out);
			else if (ast->type_out == APPEND)
				printf("  APPEND    >> : %s\n", ast->redir_out);
		}
		if (ast->pipe_out)
			printf("  PIPE to next command: |\n");
		ast = ast->next;
	}
}

void	print_tokens(t_token *tokens)
{
	int	i = 0;

	while (tokens)
	{
		printf("Token[%d]: value = \"%s\", type = %d, quote_type = %c\n", i, tokens->value, tokens->type, tokens->quote_type);
		tokens = tokens->next;
		i++;
	}
}

//lignes a supprimer dans fonction
t_ast	*ft_build_tree(char *line, t_all *all)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = NULL;
	tokens = tokenize(line, all);
	if (!tokens)
	{
		all->exit_status = 2;
		return (NULL);
	}
	free (line);
	//expand_token_values(tokens, all);
	ast = parse_ast(tokens, all);
	if (!ast)
		return (NULL);
	//print_ast(all->ast);
	printf("🔍 AST créé : %s\n", all->ast ? all->ast->cmd[0] : "NULL");

	free_token_list(tokens);
	return (ast);
}

t_token	*tokenize(char *line, t_all *all)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (line[i])
	{
		while (is_space(line[i]))
			i++;
		if (is_operator_char(line[i]))
			i = handle_operator(line, i, &tokens, all->env);
		else
		{
			i = handle_word(line, i, &tokens, all);
			if (i == -1)
			{
				free_token_list(tokens);
				all->exit_status = 2;
				return (NULL);
			}
		}
	}
	//print_tokens(tokens);
	return (tokens);
}

/* void	expand_token_values(t_token *tokens, t_all *all)
{
	while (tokens)
	{
		if (tokens->type == COMMAND && tokens->quote_type != '\'')
		{
			if (env_vars(tokens->value))
				tokens->value = fill_value_env(tokens->value, all->env);
		}
		tokens = tokens->next;
	}
} */

/* void	fill_name_file(t_ast *ast, t_token **tokens)
{
	if (ast->type == REDIR_IN || ast->type == REDIR_OUT
		|| ast->type == APPEND || ast->type == HEREDOC)
	{
		if (tokens && (*tokens)->type == COMMAND)
			ast->filename = ft_strdup((*tokens)->value);
	}
	*tokens = (*tokens)->next;
} */

/* t_ast	*parse_ast(t_token *tokens, t_env *env)
{
	t_ast	*ast;
	t_ast	*new_node;

	ast = NULL;
	while (tokens)
	{
		if (tokens->type == COMMAND)
		{
			new_node = parse_commands_in_block(&tokens);
			if (!new_node)
				return (free_token_list(tokens), NULL);
		}
		else
		{
			new_node = new_ast_node(NULL, tokens->type);
			if (!new_node)
				return (free_token_list(tokens), NULL);
			tokens = tokens->next;
			fill_name_file(new_node, &tokens);
		}
		if (!add_back_ast(&ast, new_node, env, tokens))
			return (NULL);
	}
	//free_token_list(tokens);
	return (ast);
} */

int	count_cmd_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type == COMMAND)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

void	free_cmd_args(char **args, int count)
{
	while (--count >= 0)
		free(args[count]);
	free(args);
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
	while (i < count && *tokens && (*tokens)->type == COMMAND)
	{
		args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
		i++;
	}
	args[i] = NULL;
	return (args);
}


int	handle_command(t_token **tokens, t_ast **current_cmd, t_all *all)
{
	char	**args;
	int		count;

	count = count_cmd_tokens(*tokens);
	args = dup_cmd_tokens(tokens, count, all);
	if (!args)
		return (0);
	if (!*current_cmd)
	{
		*current_cmd = new_ast_node(args);
		if (!*current_cmd)
			return (0);
		if (!add_back_ast(all, *current_cmd, *tokens))
			return (0);
	}
	else
		(*current_cmd)->cmd = args;
	return (1);
}

void	fill_redirection(t_ast *ast, t_token *redir, char *filename)
{
	if (redir->type == REDIR_IN || redir->type == HEREDOC)
	{
		free(ast->redir_in);
		ast->redir_in = ft_strdup(filename);
		ast->type_in = redir->type;
	}
	if (redir->type == REDIR_OUT || redir->type == APPEND)
	{
		free(ast->redir_out);
		ast->redir_out = ft_strdup(filename);
		ast->type_out = redir->type;
	}
}

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

int	parse_token_for_ast(t_token **tokens, t_ast **current_cmd, t_all *all)
{
	if ((*tokens)->type == COMMAND)
	{
		if (!handle_command(tokens, current_cmd, all))
			return (0);
	}
	else if ((*tokens)->type == REDIR_IN || (*tokens)->type == REDIR_OUT
		|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
	{
		if (!handle_redirection(tokens, current_cmd, all))
			return (0);
	}
	else if ((*tokens)->type == PIPE)
	{
		if (current_cmd)
			(*current_cmd)->pipe_out = 1;
		*tokens = (*tokens)->next;
		*current_cmd = NULL;
	}
	else
		*tokens = (*tokens)->next;
	return (1);
}

int	only_space_in_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_space(str[i]))
			return (0);
		i++;
	}
	return (1);
}

t_ast	*parse_ast(t_token *tokens, t_all *all)
{
	t_ast	*current_cmd;

	current_cmd = NULL;
	while (tokens)
	{
		if (!parse_token_for_ast(&tokens, &current_cmd, all))
			return (NULL);
	}
	if (all->ast->cmd[0][0] == '\0' || only_space_in_str(all->ast->cmd[0]))
	{
		ft_putstr_fd("bash:", 2);
		ft_putstr_fd(all->ast->cmd[0], 2);
		ft_putstr_fd(" : command not found\n", 2);
		free_ast_error(all->ast);
		all->ast = NULL;
		all->exit_status = 127;
		return (NULL);
	}

	// 🔽 DEBUG ICI
	/* t_ast *tmp = all->ast;
	while (tmp)
	{
		printf("DEBUG AST → cmd: %s | pipe_out: %d\n",
			tmp->cmd ? tmp->cmd[0] : "(null)", tmp->pipe_out);
		tmp = tmp->next;
	} */
	// 🔼 FIN DEBUG
	return (all->ast);
}
