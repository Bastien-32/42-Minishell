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
// dans fill_msg_between_quotes On stocke le type de quote Trouvé. 
//Pour chaque Lettre différentes de ce quote et de $ On avance dans la chaine jusqu'a les rencontrer ou jusqu'a len 
// on rajout a token->value les lettres qu'on a avancé jusqe la

int	fill_value_env(t_token **tokens, char *line, int posi, t_env *env)
{
	int		read_pos;
	int		i;
	char	*env_value_tmp_token;
	char	*env_key;

	read_pos = 1;
	env_value_tmp_token = ft_calloc(1, 1);
	if (!ft_isalpha(line[posi + read_pos]) || line[posi + read_pos] != '_')
		return (3);
	while (line[posi + read_pos]
		&& (ft_isalnum(line[posi + read_pos]) || line[posi + read_pos] == '_'))
		read_pos++;
	env_key = ft_strndup(&line[posi + 1], read_pos);
	(*tokens)->value = replace_env_key(env_key, (*tokens)->value, env);
	free(env_key);
	return (read_pos);
}

void	fill_tok_between_quotes(char *line, int i, int *read_pos, t_token **tokens, t_env *env)
{
	(*tokens)->quote_type = line[i + *read_pos];
	*read_pos += 1;
	if (line[*read_pos] == '?')
	{
		(*tokens)->value = ft_strjoin_free_s1((*tokens)->value, ft_itoa(g_exit_status));
		(*read_pos)++;
		return ;
	}
	while (line[i + *read_pos]
		&& line[i + *read_pos] != (*tokens)->quote_type)
	{
		if (line[i + *read_pos] == '$')
			*read_pos += fill_value_env(tokens, line, i + read_pos, env);
		else
		{
			(*tokens)->value = ft_strjoin_free_s1((*tokens)->value,
				ft_strndup(&line[i + *read_pos], 1));
			*read_pos += 1;
		}
	}
	*read_pos += 1;
}

char	*add_key_value(char *line, int i, int *read_pos, t_token **tokens, t_env *env)
{
	int		i;
	char	*env_key;
	char	*new_tok;

	(*read_pos)++;
	if (line[i + *read_pos] == '?')
	{
		(*tokens)->value = ft_strjoin_free_s1((*tokens)->value, ft_itoa(g_exit_status));
		(*read_pos)++;
		return ;
	}
	// À reprendre la partie d'ici ! 
	while (line[*read_pos + i] && (ft_isalnum(line[*read_pos + i])
			|| val_tok[*read_pos + i] == '_'))
		i++;
	env_key = ft_strndup(&val_tok[*read_pos], i);

	new_tok = replace_env_key(env_key, str_before_env, env);
	free(env_key);
	*read_pos += i;
	return (new_tok);
}

int	handle_word(char *line, int i, t_token **tokens, t_env *env)
{
	int		len;
	int		read_pos;

	read_pos = 0;
	(*tokens)->value = ft_calloc(1, 1);
	len = get_word_length(line, i);
	if (len == -1)
		return (-1);
	while (read_pos < len)
	{
		if (line[i + read_pos] == '\"' || line[i + read_pos] == '\'')
			fill_tok_between_quotes(line, i, &read_pos, tokens, env);
		else if (line[i + read_pos] == '$')
			read_pos = add_key_value(line, i, read_pos, tokens, env);
		else
		{
			(*tokens)->value = ft_strjoin_free_s1((*tokens)->value,
				ft_strndup(line[i + read_pos], 1));
			read_pos++;
		}
		if (read_pos == -1)
			return (-1);
	}
	return (i + len);
}
