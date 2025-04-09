#include "bastien.h"

t_ast	*new_ast_node(char **value, t_type type)
{
	t_ast *ast;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->cmd = value;
	ast->type = type;
	ast->left = NULL;
	ast->right = NULL;
	ast->filename = NULL;
	return (ast);
}

void	free_ast_error(t_ast *ast)
{
	if (ast == NULL)
		return ;
	free_ast_error(ast->left);
	free_ast_error(ast->right);
	if (ast->cmd)
	{
		int i = 0;
		while (ast->cmd[i])
			free(ast->cmd[i++]);
		free(ast->cmd);
	}
	free(ast);
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
	if(!*ast)
	{
		*ast = new;
		return (1);
	}
	else
	{
		tmp = *ast;
		while (tmp->right)
			tmp = tmp->right;
		new->left = tmp;
		new->right = NULL;
		tmp->right = new;
	}
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
	if(!cmd)
		return (write(2, "Malloc cmd failed\n", 18), NULL);
	i = 0;
	while (i < count && *tokens && (*tokens)->type == COMMAND)
	{
		cmd[i] = ft_strdup((*tokens)->value);
		if (!cmd[i]) // 5. Gestion d'erreur si strdup échoue
		{
			while (--i >= 0) // 6. Libérer les précédentes allocations
				free(cmd[i]);
			free(cmd);
			return (write(2, "ft_strdup failed\n", 17), NULL);
		}
		i++;
		*tokens = (*tokens)->next;
	}
	cmd[i] = NULL;
	return (new_ast_node(cmd, COMMAND));
}
