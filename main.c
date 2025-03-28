#include "bastien.h"

int main(void)
{
	//const char *input = "\"salut les gars $USER$HOME\"";
	const char *input = "echo \"\" | grep \'\'";
	//const char *input = "echo \"salut les gars $USER $USER\" | cat > out.txt";
	//const char *input = "echo salut les gars $USER $USER | cat > out.txt";
	//const char *input = "echo \'salut les gars $USER $USER\' | cat > out.txt";
	t_token *tokens;
	t_token *tmp;
	char quote;
	t_env *env;
	extern char **environ; // a ensuite supprimer pour utiliser la variable envp

	env = init_env(environ);
	//env = init_env(env);
	tokens = tokenize(input, env);
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