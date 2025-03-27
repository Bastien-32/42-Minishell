#include "bastien.h"

char	*get_env_value_list(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strcmp(env->env_keyname, name) == 0)
			return env->value;
		env = env->next;
	}
	return (NULL);
}

void	replace_env_value(t_token *token, char *val, char *dollar, int len)
{
	char	*before;
	char	*after;
	char	*new;

	before = strndup(token->value, dollar - token->value);
	after = strdup(dollar + 1 + len);
	new = malloc(strlen(before) + strlen(val) + strlen(after) + 1);
	if (new)
	{
		new[0] = '\0';
		strcat(new, before);
		strcat(new, val);
		strcat(new, after);
		free(token->value);
		token->value = new;
	}
	free(before);
	free(after);
}

void	expand_token_values(t_token *tokens, t_env *env)
{
	char	*dollar;
	char	*var;
	char	*val;
	int		len;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD && tokens->quote_type != '\'')
		{
			dollar = ft_strchr(tokens->value, '$');
			if (dollar && *(dollar + 1))
			{
				len = 0;
				while (dollar[1 + len]
					&& (ft_isalnum(dollar[1 + len]) || dollar[1 + len] == '_'))
					len++;
				var = ft_strndup(dollar + 1, len);
				val = get_env_value_list(env, var);
				if (val)
					replace_env_value(tokens, val, dollar, len);
				free(var);
			}
		}
		tokens = tokens->next;
	}
}
