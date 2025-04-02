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

int	handle_word(const char *line, int i, t_token **tokens, t_env *env)
{
	int		len;
	char	*string_before_cleaning;
	char	*str_cleaned;
	char	quote_type;

	len = get_word_length(line, i, *tokens, env);
	string_before_cleaning = ft_strndup(&line[i], len);
	quote_type = fill_quote_type(string_before_cleaning);
	str_cleaned = clean_quotes(string_before_cleaning, *tokens, env);
	add_token_back(tokens, new_token(str_cleaned, COMMAND, quote_type), env);
	free(string_before_cleaning);
	free(str_cleaned);
	return (i + len);
}




