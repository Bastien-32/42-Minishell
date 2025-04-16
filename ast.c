#include "bastien.h"

t_ast	*new_ast_node(char **value)
{
	t_ast	*ast;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->cmd = value;
	ast->redir_in = NULL;
	ast->redir_out = NULL;
	ast->type_in = 0;
	ast->type_out = 0;
	ast->pipe_out = 0;
	ast->next = NULL;
	return (ast);
}

void	free_ast_error(t_ast *ast)
{
	int	i;

	if (ast == NULL)
		return ;
	while (ast)
	{
		t_ast *next = ast->next;
		if (ast->cmd)
		{
			i = 0;
			while (ast->cmd[i])
				free(ast->cmd[i++]);
			free(ast->cmd);
		}
		if (ast->redir_in)
			free(ast->redir_in);
		if (ast->redir_out)
			free(ast->redir_out);
		free(ast);
		ast = next;
	}
}

int	clean_ast_and_exit(t_ast *ast, t_env *env, t_token *tokens)
{
	free_env_error(env, 0);
	free_tokens_error(tokens, 0);
	free_ast_error(ast);
	return (0);
}

int	add_back_ast(t_ast **ast, t_ast *new, t_env *env, t_token *token)
{
	t_ast	*tmp;

	if (!new)
	{
		free_env_error(env, 0);
		free_tokens_error(token, 0);
		free_ast_error(*ast);
		return (0);
	}
	if (!*ast)
	{
		*ast = new;
		return (1);
	}
	tmp = *ast;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (1);
}


t_ast	*parse_commands_in_block(t_token **tokens)
{
	t_token	*tmp;
	int		count;
	char	**cmd;
	int		i;

	count = 0;
	tmp = *tokens;
	while (tmp && tmp->type == COMMAND)
	{
		count++;
		tmp = tmp->next;
	}
	cmd = malloc (sizeof(char *) * (count + 1));
	if (!cmd)
		return (write(2, "Malloc cmd failed\n", 18), NULL);
	i = 0;
	while (i < count && *tokens && (*tokens)->type == COMMAND)
	{
		cmd[i++] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	cmd[i] = NULL;
	return (new_ast_node(cmd));
}
