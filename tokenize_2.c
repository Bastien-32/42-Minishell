#include "bastien.h"

void	handle_dollar(char *line, int *read_pos, t_token *tokens, t_all *all)
{
	int		i;
	char	next;

	i = tokens->posi;
	next =  line[i + *read_pos + 1];
	if ((next == '\"' || next == ' ') && tokens->quote_type == '\"')
	{
		append_char_to_token(tokens, '$');
		*read_pos += 1;
	}
	else if (tokens->quote_type == '\"')
	{
		*read_pos += fill_value_env(tokens, line, i + *read_pos, all);
	}
	else
	{
		append_char_to_token(tokens, '$');
		*read_pos += 1;
	}
}

void	fill_tok_between_quotes(char *line, int *read_pos,
	t_token *tokens, t_all *all)
{
	int	i;

	i = tokens->posi;
	tokens->quote_type = line[i + *read_pos];
	(*read_pos)++;

	while (line[i + *read_pos] && line[i + *read_pos] != tokens->quote_type)
	{
		if (line[i + *read_pos] == '$')
			handle_dollar(line, read_pos, tokens, all);
		else
		{
			append_char_to_token(tokens, line[i + *read_pos]);
			(*read_pos)++;
		}
	}
	if (line[i + *read_pos])
		(*read_pos)++;
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

int	parse_word(char *line, int *read_pos, t_token *token, t_all *all)
{
	char	curr;
	char	next;
	
	curr = line[token->posi + *read_pos];
	next = line[token->posi + *read_pos + 1];
	if (curr == '\"' || curr == '\'')
		fill_tok_between_quotes(line, read_pos, token, all);
	else if (curr == '$' && (next != ' ' && next != '\0'))
		*read_pos += add_key_value(line, token->posi + *read_pos, token, all);	
	else if (curr == '\\' && next)
	{
		append_char_to_token(token, curr);
		*read_pos += 2;
	}
	else
	{
		append_char_to_token(token, curr);
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
	new_tok = new_token("", COMMAND, 0, i);
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
		if (parse_word(line, &read_pos, new_tok, all) == -1)
			return (-1);
	}
	return (i + len);
}
