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

//lignes a supprimer dans fonction
t_ast	*ft_build_tree(char *line, t_env *env)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = NULL;
	tokens = tokenize(line, env);
	if (!tokens)
	{
		g_exit_status = 2;
		return (NULL);
	}
	free (line);
	expand_token_values(tokens, env);
	ast = parse_ast(tokens, env);
	if (!ast)
	{
		free_ast_error(ast);
		return (NULL);
	}
	free_token_list(tokens);
	//print_ast(ast);															// A supprimer

	return (ast);
}

t_token	*tokenize(char *line, t_env *env)
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
			i = handle_operator(line, i, &tokens, env);
		else
		{
			i = handle_word(line, i, &tokens, env);
			if (i == -1)
			{
				free_token_list(tokens);
				return (NULL);
			}
		}
	}
	return (tokens);
}

void	expand_token_values(t_token *tokens, t_env *env)
{
	while (tokens)
	{
		if (tokens->type == COMMAND && tokens->quote_type != '\'')
		{
			if (env_vars(tokens->value))
				tokens->value = fill_value_env(tokens->value, env);
		}
		tokens = tokens->next;
	}
}

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

char	**dup_cmd_tokens(t_token **tokens, int count)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
	{
		perror("Malloc failed");
		g_exit_status = 1;
		return (NULL);
	}
	i = -1;
	while (++i < count && *tokens && (*tokens)->type == COMMAND)
	{
		args[i] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	args[i] = NULL;
	return (args);
}

int	handle_command(t_token **tokens, t_ast **current_cmd,
	t_ast **ast, t_env *env)
{
	char	**args;
	int		count;

	count = count_cmd_tokens(*tokens);
	args = dup_cmd_tokens(tokens, count);
	if (!args)
		return (0);
	if (!*current_cmd)
	{
		*current_cmd = new_ast_node(args);
		if (!*current_cmd)
			return (0);
		add_back_ast(ast, *current_cmd, env, *tokens);
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

int	handle_redirection(t_token **tokens, t_ast **current_cmd,
	t_ast **ast, t_env *env)
{
	t_token	*redir;

	redir = *tokens;
	*tokens = (*tokens)->next;
	if (!*tokens || (*tokens)->type != COMMAND)
	{
		g_exit_status = 0;
		return (1);
	}
	if (!*current_cmd)
	{
		*current_cmd = new_ast_node(NULL);
		if (!*current_cmd)
			return (0);
		add_back_ast(ast, *current_cmd, env, *tokens);
	}
	fill_redirection(*current_cmd, redir, (*tokens)->value);
	*tokens = (*tokens)->next;
	return (1);
}

int	parse_token_for_ast(t_token **tokens, t_ast **current_cmd,
	t_ast **ast, t_env *env)
{
	if ((*tokens)->type == COMMAND)
	{
		if (!handle_command(tokens, current_cmd, ast, env))
			return (0);
	}
	else if ((*tokens)->type == REDIR_IN || (*tokens)->type == REDIR_OUT
		|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
	{
		if (!handle_redirection(tokens, current_cmd, ast, env))
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

t_ast	*parse_ast(t_token *tokens, t_env *env)
{
	t_ast	*ast;
	t_ast	*current_cmd;

	ast = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		if (!parse_token_for_ast(&tokens, &current_cmd, &ast, env))
			return (NULL);
	}
	// 🔽 DEBUG ICI
	/* t_ast *tmp = ast;
	while (tmp)
	{
		printf("DEBUG AST → cmd: %s | pipe_out: %d\n",
			tmp->cmd ? tmp->cmd[0] : "(null)", tmp->pipe_out);
		tmp = tmp->next;
	} */
	// 🔼 FIN DEBUG
	return (ast);
}
