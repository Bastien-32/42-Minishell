#include <string.h>
#include "/home/sapupier/Cursus42/Minishell/Libft/libft.h"
#include <fcntl.h>

void	parse_env_variables(char **env)
{
	int		i;
	char	*delim;
	size_t	key_len;
	char	*key;
	
	i = 0;
	while (env[i])
	{
		delim = strchr(env[i], '=');
		
		if (delim)
		{
			// L'extraction de la clé (avant le '=')
			key_len = delim - env[i];
			key = malloc(key_len + 1);
			if (!key)
			{
				exit(EXIT_FAILURE);
			}
			strncpy(key, env[i], key_len);
			key[key_len] = '\0';

			printf("Key: %s\n", key);

			free(key);
		}
		else
		{
			printf("No '=' found: %s\n", env[i]);
		}
		i++;
	}
}

int main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	parse_env_variables(env);
	return 0;
}