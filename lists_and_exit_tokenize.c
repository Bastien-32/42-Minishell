#include "bastien.h"

t_token	*new_token(char *value, t_type type, char quote_type, int i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
		return (NULL);
	token->type = type;
	token->quote_type = quote_type;
	token->posi = i;
	token->next = NULL;
	return (token);
}

void	free_tokens_error(t_token *token, int need_exit)
{
	free_token_list(token);
	if (need_exit)
	{
		write(2, "Malloc node token failed\n", 26);
		exit(1);
	}
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

void	add_token_back(t_token **token, t_token *new, t_env *env)
{
	t_token	*tmp;

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
