#ifndef BASTIEN_H
# define BASTIEN_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

# include "libft.h"

extern volatile sig_atomic_t	g_sigint_received;

typedef enum	e_token_type
{
	COMMAND,
	PIPE,		// |
	REDIR_IN,	// <
	REDIR_OUT,	// >
	APPEND,		// >>
	HEREDOC		// <<
}	t_type;

typedef struct	s_token
{
	char			*value;
	t_type			type;
	char			quote_type;
	struct s_token	*next;
	int				posi;
}					t_token;

typedef struct s_env
{
	char			*env_keyname;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_ast
{
	char			**cmd;
	char			*redir_in;
	char			*redir_out;
	t_type			type_in;
	t_type			type_out;
	int				pipe_out;
	struct s_ast	*next;
}	t_ast;

typedef struct s_all {
	t_env	*env;
	t_ast	*ast;
	int		exit_status;
	char	**lines;
}			t_all;

/* ****************************************************************************
									ast.c
**************************************************************************** */

t_ast	*new_ast_node(char **value);
int		clean_ast_and_exit(t_ast *ast, t_env *env, t_token *tokens);
int		add_back_ast(t_all *all, t_ast *new, t_token *token);
t_ast	*parse_commands_in_block(t_token **tokens);

/* ****************************************************************************
								 build_tree.c
**************************************************************************** */

t_ast	*ft_build_tree(char *line, t_all *all);
t_token	*tokenize(char *line, t_all *all);
int		parse_token_for_ast(t_token **tokens, t_ast **current_cmd, t_all *all);
int		only_space_in_str(char *str);
t_ast	*parse_ast(t_token *tokens, t_all *all);

/* ****************************************************************************
								env.c
**************************************************************************** */

t_env	*create_env_node(const char *env_keyname, const char *value);
void	add_env_back(t_env **env, t_env *new);
void	create_shlvl(t_env **env);
void	handle_shlvl(t_env **env);
t_env	*init_env(char **envp);

/* ****************************************************************************
								 execute_ast.c
**************************************************************************** */

int		is_valid_command(char **cmd);
int		execute_command(t_ast *node, t_all *all);
int	execute_ast_loop(t_all *all, t_ast *start, int *fd_in, pid_t *last_pid);
int		execute_cmd_without_pipe(t_ast *ast, t_all *all);
void	exit_child_builtin(t_all *all, int exit_status);
void	execute_command_child(t_ast *node, t_all *all);
int		return_error_restore_fds(t_all *all,int stdin_tmp, int stdout_tmp);
void	restore_std_and_wait_all_children(t_all *all ,pid_t last_pid, int stdin_tmp, int stdout_tmp);
int		execute_ast(t_all *all);
void	execute_cmd_followed_by_pipe(t_ast *node, t_all *all, int *fd_in, int *pipe_fd);
void	execute_last_cmd(t_all *all, t_ast *node, int *fd_in);
int		cmd_followed_by_pipe(t_all *all, t_ast **node, int *fd_in, int *pipe_fd);
int		execute_cmd_with_pipe(t_ast **ast_ptr, t_all *all, int *fd_in, pid_t *last_pid);

/* ****************************************************************************
						  execute_command_builtin.c
**************************************************************************** */

int		node_builtin(char *name_cmd);
int		execute_builtin(t_ast *node, t_all *all);

/* ****************************************************************************
						  execute_command_external.c
**************************************************************************** */

char	**parse_path(t_env *env, char *key);
char	*find_path(char *cmd, t_env *env);
void	free_array_envp(char **envp);
int		nb_env(t_env *env);
char	**env_to_array(t_env *env);
int		prepare_env_and_path(t_all *all, t_ast *node, char **cmd_path, char ***envp);
void	exec_child_process(char *cmd_path, t_ast *ast, char **envp);
void	wait_child_status(t_all *all, int pid);
int		execute_external(t_ast *node, t_all *all);

/* ****************************************************************************
						  execute_redirection.c
**************************************************************************** */

int		perror_message(t_all *all,char *str);
int		ft_redir_in(t_ast *node, t_all *all);
int		ft_redir_out(t_ast *node, t_all *all);
int		ft_append(t_ast *node, t_all *all);
int		put_heredoc_in_stdin(t_all *all);								//a supprimer je pense
int		execute_redirection(t_ast *node, t_all *all);

/* ****************************************************************************
								free.c
**************************************************************************** */

void	free_ast_node(t_ast *ast);
void	free_ast_error(t_ast *ast);
void	free_env_error(t_env *env, int need_exit);
void	free_env_list(t_env *env);

/* ****************************************************************************
						  			handle_cmds.c
**************************************************************************** */

int		handle_redirection_in_command(t_token *tokens, t_ast **current_cmd,
			t_all *all);
int		count_cmd_tokens(t_token *tokens, t_ast **current_cmd, t_all *all);
char	**dup_cmd_tokens(t_token **tokens, int count, t_all *all);
void	free_cmd_args(char **args, int count);
int		get_args_and_count(t_token **tokens, t_ast **current_cmd, 
			t_all *all, char ***args_out);

/* ****************************************************************************
						   list_and_exit_tokenize.c
**************************************************************************** */

t_token	*new_token(char *value, t_type type, char quote_type, int i);
void	free_tokens_error(t_token *token, int need_exit);
void	free_token_list(t_token *token);
void	clean_all_and_exit(t_env *env, t_token *tokens);
void	add_token_back(t_token **token, t_token *new, t_env *env);

/* ****************************************************************************
							  parse_token_for_ast.c
**************************************************************************** */

int	handle_redirection(t_token **tokens, t_ast **current_cmd, t_all *all);
int	handle_command(t_token **tokens, t_ast **current_cmd, t_all *all);
void	fill_redirection(t_ast *ast, t_token *redir, char *filename);

/* ****************************************************************************
								  tokenize.c
**************************************************************************** */

int		handle_operator(const char *line, int i, t_token **tokens, t_env *env);
int		get_word_length(const char *line, int start);
char	*clean_quotes(const char *str, t_token *tokens, t_env *env);
char	fill_quote_type(const char *str);
void	replace_env_key(char *env_key, t_token *token, t_all *all);
int		fill_value_env2(t_token *tokens, char *line, int posi, t_all *all); 		// a renommer si pas de env1
void	append_char_to_token(t_token *tokens, char c);
void	handle_dollar(char *line, int *read_pos, t_token *tokens, t_all *all);
void	fill_tok_between_quotes(char *line, int *read_pos, t_token *tokens, t_all *all);
int		add_key_value(char *line, int ipos, t_token *tokens, t_all *all);
int		parse_word(char *line, int *read_pos, t_token *token, t_all *all);
int		handle_word(char *line, int i, t_token **tokens, t_all *all);

/* ****************************************************************************
									utils.c
**************************************************************************** */

char	*ft_strjoin_free_all(char *s1, char *s2);
char	*ft_strjoin_free_s1(char *s1, char *s2);
int		is_space(char c);
int		is_operator_char(char c);

/* ****************************************************************************
									utils_cd
**************************************************************************** */

char	*get_env_value(t_env *env, const char *key);

/* ****************************************************************************
									builtins
**************************************************************************** */

int		env_builtin(t_ast *node, t_all *all);
int		unset_builtin(t_ast *node, t_all *all);
int		exit_builtin(t_ast *node, t_all *all);
void	free_tab(char **tab);

int		echo_builtin(t_ast *node, t_all *all);
int		change_pwd(t_all *all);
int		cd_builtin(t_ast *node, t_all *all);
int		pwd_builtin(t_all *all);
int		export_builtin(t_ast *node, t_all *all);
int		export_sorted(t_env *env);


void	print_ast(t_ast *ast);

//void	handle_signals(void);
void	setup_signals_child(void);
void	setup_signals_parent(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

void free_heredocs_temp(t_ast *ast);


# endif
