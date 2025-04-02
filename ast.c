#include "bastien.h"

t_ast	*new_ast_node(const char **value, t_type type)
{
	t_ast *ast;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->cmd = (char **)value;
	ast->type = type;
	ast->left = NULL;
	ast->right = NULL;
	return (ast);
}

void	free_ast_error(t_ast *ast, int need_exit)
{
	t_ast	*tmp;
	int		j;

	while (ast)
	{
		tmp = ast->left;
		if (ast->cmd)
		{
			j = 0;
			while (ast->cmd[j])
				free(ast->cmd[j++]);
			free(ast->cmd);
		}
		ast = tmp;
	}
	if (need_exit)
	{
		write(2, "Malloc node ast failed\n", 24);
		exit(1);
	}
}

void	clean_ast_and_exit(t_ast *ast, t_env *env, t_token *tokens)
{
	free_env_error(env, 0);
	free_tokens_error(tokens, 0);
	free_ast_error(ast, 1);
}

void	add_back_ast(t_ast **ast, t_ast *new, t_env *env, t_token *token)
{
	t_ast	*tmp;

	if (!new)
		clean_ast_and_exit(*ast, env, token);
	if(!*ast)
	{
		*ast = new;
		return ;
	}
	else
	{
		tmp = *ast;
		while (tmp->right)
			tmp = tmp->right;
		new->left = tmp;
		tmp->right = new;
	}
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
	if(!cmd)
		return (NULL);
	i = 0;
	while (i < count && *tokens && (*tokens)->type == COMMAND)
	{
		cmd[i++] = ft_strdup((*tokens)->value);
		*tokens = (*tokens)->next;
	}
	cmd[i] = NULL;
	return (new_ast_node((const char **)cmd, COMMAND)); 
}
