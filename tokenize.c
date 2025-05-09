#include "bastien.h"

int	handle_operator(const char *line, int i, t_token **tokens, t_env *env)
{
	if (line[i] == '|')
		add_token_back(tokens, new_token("|", PIPE, 0), env);
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token_back(tokens, new_token("<<", HEREDOC, 0), env);
			i++;
		}
		else
			add_token_back(tokens, new_token("<", REDIR_IN, 0), env);
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token_back(tokens, new_token(">>", APPEND, 0), env);
			i++;
		}
		else
			add_token_back(tokens, new_token(">", REDIR_OUT, 0), env);
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

char	*clean_quotes(const char *str, t_token *tokens, t_env *env)
{
	char	*str_cleaned;
	int		i;
	int		j;
	char	quote;

	str_cleaned = malloc(ft_strlen(str) + 1);
	if (!str_cleaned)
		clean_all_and_exit(env, tokens);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\"' || str[i] == '\'') && !quote)
			quote = str[i++];
		else if (str[i] == quote)
		{
			quote = 0;
			i++;
		}
		else
			str_cleaned[j++] = str[i++];
	}
	str_cleaned[j] = '\0';
	return (str_cleaned);
}

char	fill_quote_type(const char *str)
{
	if (str[0] == '\"' || str[0] == '\'')
		return (str[0]);
	return (0);
}

/* int	handle_word(char *line, int i, t_token **tokens, t_env *env)
{
	int		len;
	char	*string_before_cleaning;
	char	*str_cleaned;
	char	quote_type;

	len = get_word_length(line, i);
	if (len == -1)
		return (-1);
	string_before_cleaning = ft_strndup(&line[i], len);
	quote_type = fill_quote_type(string_before_cleaning);
	str_cleaned = clean_quotes(string_before_cleaning, *tokens, env);
	add_token_back(tokens, new_token(str_cleaned, COMMAND, quote_type), env);
	free(string_before_cleaning);
	free(str_cleaned);
	return (i + len);
} */

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

int	fill_value_env2(t_token *tokens, char *line, int posi, t_all *all)
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

void	fill_tok_between_quotes(char *line, int i, int *read_pos,
	t_token *tokens, t_all *all)
{
	tokens->quote_type = line[i + *read_pos];
	*read_pos += 1;
	while (line[i + *read_pos]
		&& line[i + *read_pos] != tokens->quote_type)
	{
		if (line[i + *read_pos] == '$' && line[i + *read_pos + 1] == '\"' && tokens->quote_type == '\"')
		{
			tokens->value = ft_strjoin_free_all(tokens->value,
				ft_strndup(&line[i + *read_pos], 1));
			*read_pos += 1;
		}
		else if (line[i + *read_pos] == '$' && line[i + *read_pos + 1] == ' ')
		{
			tokens->value = ft_strjoin_free_all(tokens->value,
				ft_strndup(&line[i + *read_pos], 1));
			*read_pos += 1;
		}
		else if (line[i + *read_pos] == '$' && tokens->quote_type == '\"')
			*read_pos += fill_value_env2(tokens, line, i + *read_pos, all);
		else
		{
			tokens->value = ft_strjoin_free_all(tokens->value,
				ft_strndup(&line[i + *read_pos], 1));
			*read_pos += 1;
		}
	}
	if (line[i + *read_pos])
		*read_pos += 1;
	tokens->quote_type = 0;
}

int	add_key_value(char *line, int ipos, t_token *tokens, t_all *all)
{
	int		read_pos;
	char	*env_key;

	read_pos = 1;
	if (line[ipos + read_pos] == '?')
	{
		tokens->value = ft_strjoin_free_all(tokens->value,
			ft_itoa(all->exit_status));
		return (2);
	}
	if (line[ipos + read_pos] == '\"' || line[ipos + read_pos] == '\'')
		return (1);
	if (!ft_isalpha(line[ipos + read_pos]) && line[ipos + read_pos] != '_')
		return (2);
	while (line[read_pos + ipos] && (ft_isalnum(line[read_pos + ipos])
			|| line[read_pos + ipos] == '_'))
		read_pos++;
	env_key = ft_strndup(&line[ipos + 1], read_pos - 1);
	replace_env_key(env_key, tokens, all);
	free(env_key);
	return (read_pos);
}

int	parse_word(char *line, int i, int *read_pos, t_token *token, t_all *all)
{
	char	*letter_to_dup;
	char	c[2];

	if (line[i + *read_pos] == '\"' || line[i + *read_pos] == '\'')
		fill_tok_between_quotes(line, i, read_pos, token, all);
	else if (line[i + *read_pos] == '$' &&
		(line[i + *read_pos + 1] == ' ' || line[i + *read_pos + 1] == '\0'))
	{
		token->value = ft_strjoin_free_all(token->value,
			ft_strndup(&line[i + *read_pos], 1));
		*read_pos += 1;
	}
	else if (line[i + *read_pos] == '$')
		*read_pos += add_key_value(line, i + *read_pos, token, all);
	else if (line[i + *read_pos] == '\\' && (line[i + *read_pos + 1]))
	{
		c[0] = line[i + *read_pos + 1]; 
		c[1] = '\0';
		token->value = ft_strjoin_free_s1(token->value, ft_strdup(c));
		*read_pos += 2;
	}
	else
	{
		letter_to_dup = ft_strndup(&line[i + *read_pos], 1);
		token->value = ft_strjoin_free_s1(token->value, letter_to_dup);
		free(letter_to_dup);
		(*read_pos)++;
	}
	if (*read_pos == -1)
		return (-1);
	return (0);
}

int	handle_word(char *line, int i, t_token **tokens, t_all *all)
{
	int			len;
	int			read_pos;
	t_token		*new_tok;

	read_pos = 0;
	new_tok = new_token("", COMMAND, 0);
	if (!new_tok)
		return (-1);
	add_token_back(tokens, new_tok, all->env);
	if (!new_tok->value)
		return (-1);
	len = get_word_length(line, i);
	if (len == -1)
		return (-1);
	while (read_pos < len)
	{
		if (parse_word(line, i, &read_pos, new_tok, all) == -1)
			return (-1);
	}
	return (i + len);
}
