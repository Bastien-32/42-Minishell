#include "bastien.h"

t_token *new_token(const char *value, t_token_type type, char quote_type)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->quote_type = quote_type;
	token->next = NULL;
	return token;
}

void add_token(t_token **head, t_token *new)
{
	t_token *tmp;

	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int handle_operator(const char *line, int i, t_token **tokens)
{
	if (line[i] == '|')
		add_token(tokens, new_token("|", TOKEN_PIPE, 0));
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(tokens, new_token("<<", TOKEN_HEREDOC, 0));
			i++;
		}
		else
			add_token(tokens, new_token("<", TOKEN_REDIR_IN, 0));
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(tokens, new_token(">>", TOKEN_APPEND, 0));
			i++;
		}
		else
			add_token(tokens, new_token(">", TOKEN_REDIR_OUT, 0));
	}
	return (i + 1);
}

int get_word_length(const char *line, int start)
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
				errno = 22;
				perror("Usage : Wrong close of quotes");
				return (-1);
			}
			i++;
		}
		else
			i++;
	}
	return (i - start);
}

char *clean_quotes(const char *str)
{
	char	*string_cleaned;
	int		i;
	int		j;
	char	quote;

	string_cleaned = malloc(ft_strlen(str) + 1);
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

char fill_quote_type(const char *str)
{
	if (str[0] == '\"' || str[0] == '\'')
		return (str[0]);
	return (0);
}

int handle_word(const char *line, int i, t_token **tokens)
{
	int		len;
	char	*string_before_cleaning;
	char	*string_cleaned;
	char	quote_type;

	len = get_word_length(line, i);
	if (len < 0)
		return (-1);
	string_before_cleaning = ft_strndup(&line[i], len);
	quote_type = fill_quote_type(string_before_cleaning);
	string_cleaned = clean_quotes(string_before_cleaning);
	add_token(tokens, new_token(string_cleaned, TOKEN_WORD, quote_type));
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
	while (line[i])
	{
		if (is_space(line[i]))
			i++;
		if (is_operator_char(line[i]))
			i = handle_operator(line, i, &tokens);
		else
		{
			i = handle_word(line, i, &tokens);
			if (i < 0)
				return (NULL);
		}
	}
	expand_token_values(tokens, env);
	return (tokens);
}

int main(void)
{
	const char *input = "echo \"salut les gars $USER $USER\" | cat > out.txt";
	const char *input = "echo   salut les gars $USER $USER   | cat > out.txt";
	const char *input = "echo \'salut les gars $USER $USER\' | cat > out.txt";
	t_token *tokens;
	t_token *tmp;
	char quote;
	t_env *env;

	env = init_env(&env);
	tokens = tokenize(input, &env);
	tmp = tokens;
	while (tmp)
	{
		if (tmp->quote_type)
			quote = tmp->quote_type;
		else
			quote = ' ';
		printf("type: %d | quote: %c | value: \"%s\"\n",
			tmp->type,
			quote,
			tmp->value);
		tmp = tmp->next;
	}

	return (0);
}