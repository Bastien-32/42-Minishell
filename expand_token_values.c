#include "bastien.h"

int	first_env_char(char c)
{
	if (c == '?' || ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

int	env_vars(char *value_token)
{
	int	i;

	i = 0;
	while (value_token[i])
	{
		if (value_token[i] == '$' && first_env_char(value_token[i + 1]))
			return (1);
		i++;
	}
	return (0);
}

char	*replace_env_key(char *env_key, char *str_before_env, t_env *env)
{
	t_env	*temp_env;

	temp_env = env;
	while (temp_env)
	{
		if (ft_strcmp(temp_env->env_keyname, env_key) == 0)
			return (ft_strjoin_free_s1(str_before_env, temp_env->value));
		temp_env = temp_env->next;
	}
	return (ft_strjoin_free_s1(str_before_env, ""));
}

char	*add_key_value(char *val_tok, int *read_pos,
	char *str_before_env, t_env *env)
{
	int		i;
	char	*env_key;
	char	*new_tok;

	(*read_pos)++;
	if (val_tok[*read_pos] == '?')
	{
		new_tok = ft_strjoin_free_s1(str_before_env, ft_itoa(g_exit_status));
		(*read_pos)++;
		return (new_tok);
	}
	i = 0;
	while (val_tok[*read_pos + i] && (ft_isalnum(val_tok[*read_pos + i])
			|| val_tok[*read_pos + i] == '_'))
		i++;
	env_key = ft_strndup(&val_tok[*read_pos], i);

	new_tok = replace_env_key(env_key, str_before_env, env);
	free(env_key);
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
	while (value_token[read_pos])
	{
		i = 0;
		while (value_token[read_pos + i] && value_token[read_pos + i] != '$')
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
