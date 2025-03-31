#include "bastien.h"

t_token	*new_token(const char *value, t_token_type type, char quote_type)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
		return (NULL);
	token->type = type;
	token->quote_type = quote_type;
	token->next = NULL;
	return token;
}

void	free_tokens_error(t_token *token, int need_exit)
{
	free_token_list(token);
	write(2, "Malloc node token failed\n", 26);
	if (need_exit)
		exit(1);
}

void	free_token_list(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

void	clean_all_and_exit(t_env *env, t_token *tokens)
{
	free_env_error(env, 0);
	free_tokens_error(tokens, 1);
}

void	add_token(t_token **token, t_token *new, t_env *env)
{
	t_token *tmp;

	if (!new)
		clean_all_and_exit(env, *token);
	if (!*token)
		*token = new;
	else
	{
		tmp = *token;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	handle_operator(const char *line, int i, t_token **tokens, t_env *env)
{
	if (line[i] == '|')
		add_token(tokens, new_token("|", TOKEN_PIPE, 0), env);
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(tokens, new_token("<<", TOKEN_HEREDOC, 0), env);
			i++;
		}
		else
			add_token(tokens, new_token("<", TOKEN_REDIR_IN, 0), env);
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(tokens, new_token(">>", TOKEN_APPEND, 0), env);
			i++;
		}
		else
			add_token(tokens, new_token(">", TOKEN_REDIR_OUT, 0), env);
	}
	return (i + 1);
}

int	get_word_length(const char *line, int start, t_token *tokens, t_env *env)
{
	int i;
	char quote;

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
				write(2, "wrong close of \'\' or \"\"\n", 25); //voir s'il fat gerer comme dans le vrais terminal
				free_token_list(tokens);
				free_env_list(env);
				exit(1);
			}
			i++;
		}
		else
			i++;
	}
	return (i - start);
}

char	*clean_quotes(const char *str,  t_token *tokens, t_env *env)
{
	char	*string_cleaned;
	int		i;
	int		j;
	char	quote;

	string_cleaned = malloc(ft_strlen(str) + 1);
	if (!string_cleaned)
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
			string_cleaned[j++] = str[i++];
	}
	string_cleaned[j] = '\0';
	return (string_cleaned);
}

char	fill_quote_type(const char *str)
{
	if (str[0] == '\"' || str[0] == '\'')
		return (str[0]);
	return (0);
}

int	handle_word(const char *line, int i, t_token **tokens, t_env *env)
{
	int		len;
	char	*string_before_cleaning;
	char	*string_cleaned;
	char	quote_type;

	len = get_word_length(line, i, *tokens, env);
	string_before_cleaning = ft_strndup(&line[i], len);
	quote_type = fill_quote_type(string_before_cleaning);
	string_cleaned = clean_quotes(string_before_cleaning, *tokens, env);
	add_token(tokens, new_token(string_cleaned, TOKEN_WORD, quote_type), env);
	free(string_before_cleaning);
	free(string_cleaned);
	return (i + len);
}

t_token	*tokenize(const char *line, t_env *env)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	printf("line = %s\n", line);												//a supprimer pour rendu final
	while (line[i])
	{
		if (is_space(line[i]))
			i++;
		if (is_operator_char(line[i]))
			i = handle_operator(line, i, &tokens, env);
		else
			i = handle_word(line, i, &tokens, env);
	}
	expand_token_values(tokens, env);
	return (tokens);
}

