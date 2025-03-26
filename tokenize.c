#include "bastien.h"

t_token *new_token(const char *value, t_token_type type)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->value = strdup(value);
	token->type = type;
	token->next = NULL;
	return token;
}

