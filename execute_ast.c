#include "bastien.h"

int	execute_command(t_ast *ast, t_env *env)
{
	if (node_builtin(ast->cmd[0]))
		execute_builtin(ast);
	else
		execute_external(ast, env);
}

int	execute_ast(t_ast *ast, t_env *env)
{
	while (ast)
	{
		if (ast->type == COMMAND)
			execute_command(ast, env);
		else if (ast->type == PIPE)
			execute_pipe(ast, env);
		else if (ast->type == REDIR_IN || ast->type == REDIR_OUT || ast->type == APPEND)
			execute_redirection(ast, env);
		else if (ast->type == HEREDOC)
			execute_heredoc(ast, env);
		ast = ast->right;
	}
	return (0);
}
