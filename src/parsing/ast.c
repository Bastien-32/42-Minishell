/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:43:33 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/15 18:43:34 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	clean_ast_and_exit(t_ast *ast, t_env *env, t_token *tokens)
{
	free_env_error(env, 0);
	free_tokens_error(tokens, 0);
	free_ast_error(ast);
	return (0);
}

int	add_back_ast(t_all *all, t_ast *new, t_token *token)
{
	t_ast	*tmp;

	if (!new)
	{
		free_tokens_error(token, 0);
		free_ast_error(all->ast);
		free(all);
		return (0);
	}
	if (!all->ast)
	{
		all->ast = new;
		return (1);
	}
	tmp = all->ast;
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
