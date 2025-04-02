#include "bastien.h"

int	env_vars(char *value_token)
{
	int	i;

	i = 0;
	while (value_token[i])
	{
		if (value_token[i] == '$' && value_token[i + 1])
			return (1);
		i++;
	}
	return (0);
}

char	*add_key_value(char *val_tok, int *read_pos, char *old_tok, t_env *env)
{
	int		i;
	char	*env_key;
	char	*new_tok;
	t_env	*temp_env;

	(*read_pos)++;
	i = 0;
	while (val_tok[*read_pos + i] && (ft_isalnum(val_tok[*read_pos + i])
		|| val_tok[*read_pos + i] == '_'))
		i++;
	env_key = ft_strndup(&val_tok[*read_pos], i);
	temp_env = env;
	while (temp_env)
	{
		if (ft_strcmp(temp_env->env_keyname, env_key) == 0)
		{
			new_tok = ft_strjoin_free_s1(old_tok, temp_env->value);
			break ;
		}
		temp_env = temp_env->next;
	}
	if (!temp_env)
		new_tok = ft_strjoin_free_s1(old_tok, "");
	*read_pos += i;
	return (new_tok);
}

char	*fill_value_env(char *value_token, t_env *env)
{
	int		read_pos;
	int		i;
	char	*new_token;
	char	*temp;

	read_pos = 0;
	new_token = ft_calloc(1, 1);
	while(value_token[read_pos])
	{
		i = 0;
		while(value_token[read_pos + i] && value_token[read_pos + i] != '$')
			i++;
		if (i > 0)
		{
			temp = ft_strndup(&value_token[read_pos], i);
			new_token = ft_strjoin_free_all(new_token, temp);
			read_pos += i;
		}
		if (value_token[read_pos] == '$')
			new_token = add_key_value(value_token, &read_pos, new_token, env);
	}
	return (new_token);
}

void	expand_token_values(t_token *tokens, t_env *env)
{
	while (tokens)
	{
		if (tokens->type == COMMAND && tokens->quote_type != '\'')
		{
			if (env_vars(tokens->value))
				tokens->value = fill_value_env(tokens->value, env);
		}
		tokens = tokens->next;
	}
}
