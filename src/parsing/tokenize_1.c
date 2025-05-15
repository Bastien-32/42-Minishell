/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:44:12 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/15 18:44:13 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_operator(const char *line, int i, t_token **tokens, t_env *env)
{
	if (line[i] == '|')
		add_token_back(tokens, new_token("|", PIPE, 0, i), env);
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token_back(tokens, new_token("<<", HEREDOC, 0, i), env);
			i++;
		}
		else
			add_token_back(tokens, new_token("<", REDIR_IN, 0, i), env);
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token_back(tokens, new_token(">>", APPEND, 0, i), env);
			i++;
		}
		else
			add_token_back(tokens, new_token(">", REDIR_OUT, 0, i), env);
	}
	return (i + 1);
}

int	get_word_length(const char *line, int start)
{
	int		i;
	char	quote;

	i = start;
	while (line[i] && !is_space(line[i]) && !is_operator_char(line[i]))
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
			{
				write(2, "wrong close of \'\' or \"\"\n", 25);
				return (-1);
			}
			i++;
		}
		else
			i++;
	}
	return (i - start);
}

void	replace_env_key(char *env_key, t_token *token, t_all *all)
{
	t_env	*temp_env;

	temp_env = all->env;
	while (temp_env)
	{
		if (ft_strcmp(temp_env->env_keyname, env_key) == 0)
		{
			token->value = ft_strjoin_free_s1(token->value,
					temp_env->value);
			return ;
		}
		temp_env = temp_env->next;
	}
	token->value = ft_strjoin_free_s1(token->value, "");
}

int	fill_value_env(t_token *tokens, char *line, int posi, t_all *all)
{
	int		read_pos;
	char	*env_key;

	read_pos = 1;
	if (line[posi + read_pos] == '?')
	{
		tokens->value = ft_strjoin_free_all(tokens->value,
				ft_itoa(all->exit_status));
		return (2);
	}
	if (!ft_isalpha(line[posi + read_pos]) && line[posi + read_pos] != '_')
		return (2);
	while (line[posi + read_pos]
		&& (ft_isalnum(line[posi + read_pos]) || line[posi + read_pos] == '_'))
		read_pos++;
	env_key = ft_strndup(&line[posi + 1], read_pos - 1);
	replace_env_key(env_key, tokens, all);
	free(env_key);
	return (read_pos);
}

void	append_char_to_token(t_token *tokens, char c)
{
	char	str[2];

	str[0] = c;
	str[1] = '\0';
	tokens->value = ft_strjoin_free_all(tokens->value, ft_strdup(str));
}
