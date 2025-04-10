#include "bastien.h"

//fonction a supprimer
void	print_ast(t_ast *ast)
{
	printf("abstract syntax tree :\n");
	while (ast)
	{
		if (ast->type == COMMAND)
		{
			printf("Make command :\n");
			int i = 0;
			while (ast->cmd[i])
			{
				printf("	CMD : %s\n", ast->cmd[i]);
				i++;
			}
		}
		else if (ast->type == PIPE)
			ft_printf("PIPE |\n");
		else if (ast->type == REDIR_IN)
			ft_printf("REDIR_IN <\n	FILENAME : %s\n", ast->filename);
		else if (ast->type == REDIR_OUT)
			ft_printf("REDIR_OUT >\n	FILENAME : %s\n", ast->filename);
		else if (ast->type == APPEND)
			ft_printf("APPEND >>\n	FILENAME : %s\n", ast->filename);
		else if (ast->type == HEREDOC)
			ft_printf("HEREDOC <<\n	DELIMITER : %s\n", ast->filename);
		else
			printf("UNKNOWN\n");
		ast = ast->right;
	}
}

//lignes a supprimer dans fonction
t_ast	*build_tree(char *line, t_env *env)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = NULL;

	//printf("line = %s\n", line);												//a supprimer pour rendu final

	tokens = tokenize(line, env);
	if (!tokens)
	{
		g_exit_status = 2;
		return (NULL);
	}
	free (line);
	expand_token_values(tokens, env);
/* 
	t_token	*tmp;																//a supprimer de là ...
	char	quote;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->quote_type)
			quote = tmp->quote_type;
		else
			quote = ' ';
		printf("type: %d | quote: %c | value: \"%s\"\n",
			tmp->type,
			quote,
			tmp->value);
		tmp = tmp->next;
	}																			// ... à là
 */
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
		if (is_space(line[i]))
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

void	fill_name_file(t_ast *ast, t_token **tokens)
{
	if (ast->type == REDIR_IN || ast->type == REDIR_OUT
		|| ast->type == APPEND || ast->type == HEREDOC)
	{
		if (tokens && (*tokens)->type == COMMAND)
			ast->filename = ft_strdup((*tokens)->value);
	}
	*tokens = (*tokens)->next;
}

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
//mettre le else if dans une fonction parse_redirs_in_block
t_ast	*parse_cmds_and_redirs_in_block(t_token **tokens)
{
	t_ast *ast = NULL;
	t_ast *last_node = NULL;
	t_ast *new_node = NULL;

	while (*tokens)
	{
		if ((*tokens)->type == COMMAND)
		{
			if (!ast)
			{
				// Première commande rencontrée = racine du bloc
				ast = parse_commands_in_block(tokens);
				last_node = ast;
			}
			else
			{
				// Commande supplémentaire (imprévu, sécurité)
				break;
			}
		}
		else if ((*tokens)->type == REDIR_IN || (*tokens)->type == REDIR_OUT
			|| (*tokens)->type == APPEND || (*tokens)->type == HEREDOC)
		{
			new_node = new_ast_node(NULL, (*tokens)->type);
			*tokens = (*tokens)->next;
			fill_name_file(new_node, tokens);
			
			if (!last_node)
			{
				// redirection sans commande → erreur
				free_ast_error(new_node);
				return (NULL);
			}
			last_node->right = new_node;
			new_node->left = last_node;
			last_node = new_node;
		}
		else
			break;
	}
	return (ast);
}

t_ast *parse_ast(t_token *tokens, t_env *env)
{
	t_ast *ast;
	t_ast *pipe_node;
	t_ast *next_block;

	if (!tokens)
		return (NULL);
	ast = parse_cmds_and_redirs_in_block(&tokens);
	while (tokens && tokens->type == PIPE)
	{
		tokens = tokens->next;
		next_block = parse_cmds_and_redirs_in_block(&tokens);
		if (!next_block)
			return (free_ast_error(ast), NULL);
		pipe_node = new_ast_node(NULL, PIPE);
		pipe_node->left = ast;
		pipe_node->right = next_block;
		ast = pipe_node;
	}
	return (ast);
}
