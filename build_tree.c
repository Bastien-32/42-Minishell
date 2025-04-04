#include "bastien.h"

void	print_ast(t_ast *ast)									//fonction a supprimer
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
		printf("PIPE |\n");
		else if (ast->type == REDIR_IN)
			printf("REDIR_IN <\n");
		else if (ast->type == REDIR_OUT)
			printf("REDIR_OUT >\n");
		else if (ast->type == APPEND)
			printf("APPEND >>\n");
		else if (ast->type == HEREDOC)
			printf("HEREDOC <<\n");
		else
			printf("UNKNOWN\n");
		ast = ast->right;
	}
}

t_ast	*build_tree(const char *line, t_env *env)
{
	t_token	*tokens;
	t_ast	*ast;

	tokens = NULL;

	//printf("line = %s\n", line);												//a supprimer pour rendu final

	tokens = tokenize(line, env);
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

	//print_ast(ast);															// A supprimer

	return (ast);
}

t_token	*tokenize(const char *line, t_env *env)
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
			i = handle_word(line, i, &tokens, env);
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

t_ast	*parse_ast(t_token *tokens, t_env *env)
{
	t_ast *ast;
	t_ast *new_node;

	ast = NULL;
	while (tokens)
	{
		if (tokens->type == COMMAND)
			new_node = parse_commands_in_block(&tokens);
		else
		{
			new_node = new_ast_node(NULL, tokens->type);
			tokens = tokens->next;
		}
		add_back_ast(&ast, new_node, env, tokens);
	}
	free_token_list(tokens);
	return (ast);
}
