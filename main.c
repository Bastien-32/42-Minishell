#include "bastien.h"

sig_atomic_t	g_exit_status = 0;


int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	t_ast	*ast;
	char	*line;

	(void)argc;
	(void)argv;

	env = init_env(envp);
	while (1)
	{
		setup_signals_main(); // ← pour réactiver les signaux à chaque tour
		line = readline("minishell> ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		ast = build_tree(line, env);
		if (ast)
		{
			execute_ast(ast, &env);
			free_ast_error(ast);
			//printf("[main loop] back to prompt\n");							//a supprimer
		}
		//free(line);
	}
	free_env_list(env);
	return (0);
}
