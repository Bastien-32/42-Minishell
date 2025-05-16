/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badal-la <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:43:39 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/16 11:06:38 by badal-la         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	ast = parse_ast(tokens, all);
	if (!ast)
		return (NULL);
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
		if (!line[i])
			return (tokens);
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
	return (tokens);
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
		/* if (!(*tokens))
			return (0); */
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
	t_token	*temp_tok;

	if (!pipe_in_first_pos(tokens, all))
		return (NULL);
	if (!nothing_after_pipe(tokens, all))
		return (free_ast_error(all->ast), free_token_list(tokens), NULL);
	current_cmd = NULL;
	temp_tok = tokens;
	while (tokens)
	{
		if (!parse_token_for_ast(&tokens, &current_cmd, all))
			return (free_ast_error(all->ast), free_token_list(temp_tok), NULL);
	}
	//if ((temp_tok->type== REDIR_IN || temp_tok->type== REDIR_OUT || temp_tok->type== APPEND || temp_tok->type== HEREDOC) && !temp_tok->next)
	if (temp_tok->type != COMMAND && !temp_tok->next)
		return (free_ast_error(all->ast), free_token_list(temp_tok), NULL);
	if ((all->ast->cmd && all->ast->cmd[0])
		&& (all->ast->cmd[0][0] == '\0' || only_space_in_str(all->ast->cmd[0])))
	{
		ft_putstr_fd(all->ast->cmd[0], 2);
		ft_putstr_fd(" : command not found\n", 2);
		free_ast_error(all->ast);
		all->ast = NULL;
		all->exit_status = 127;
		return (free_token_list(temp_tok), NULL);
	}
	return (all->ast);
}
