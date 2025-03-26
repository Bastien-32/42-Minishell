
#include "/home/sapupier/Cursus42/Minishell/Libft/libft.h"

void	parse_env_variables(char **env)
{
	int		i;
	char	*delim;
	
	i = 0;
	while (env[i])
	{
		delim = strchr(env[i], '=');
		
		if (delim)
		{
			// L'extraction de la clé (avant le '=')
			size_t key_len = delim - env[i];
			char key[key_len + 1];
			strncpy(key, env[i], key_len);
			key[key_len] = '\0';

			// Affichage de la clé
			printf("Key: %s\n", key);
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