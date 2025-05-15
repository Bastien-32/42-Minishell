/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sapupier <sapupier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:46:26 by sapupier          #+#    #+#             */
/*   Updated: 2025/05/15 18:47:02 by sapupier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef enum e_token_type
{
	COMMAND,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_type;

typedef struct s_token
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

typedef struct s_all
{
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
								   handle_signals.c
**************************************************************************** */

void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	setup_signals_child(void);
void	setup_signals_parent(void);
char	*read_input(t_all *all);

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

int		execute_ast(t_all *all);
int		execute_ast_loop(t_all *all, t_ast *start, int *fd_in, pid_t *last_pid);
int		execute_cmd_without_pipe(t_ast *ast, t_all *all);
int		execute_cmd_with_pipe(t_ast **ast_ptr, t_all *all, int *fd_in,
			pid_t *last_pid);
void	execute_last_cmd(t_all *all, t_ast *node, int *fd_in);

/* ****************************************************************************
								 execute_cmd_with_pipe.c
**************************************************************************** */

int		cmd_followed_by_pipe(t_all *all, t_ast **node, int *fd_in,
			int *pipe_fd);
void	execute_cmd_followed_by_pipe(t_ast *node, t_all *all, int *fd_in,
			int *pipe_fd);
void	execute_command_child(t_ast *node, t_all *all);
void	exit_child_builtin(t_all *all, int exit_status);

/* ****************************************************************************
								 execute_cmd_without_pipe.c
**************************************************************************** */

int		execute_command(t_ast *node, t_all *all);
int		execute_external( t_ast *node, t_all *all);
int		is_valid_command(char **cmd);

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
void	free_tab(char **envp);
int		nb_env(t_env *env);
char	**env_to_array(t_env *env);
int		prepare_env_and_path(t_all *all, t_ast *node,
			char **cmd_path, char ***envp);

/* ****************************************************************************
							execute_redirection.c
**************************************************************************** */

int		perror_message(t_all *all, char *str);
int		ft_redir_in(t_ast *node, t_all *all);
int		ft_redir_out(t_ast *node, t_all *all);
int		ft_append(t_ast *node, t_all *all);
int		execute_redirection(t_ast *node, t_all *all);

/* ****************************************************************************
									free.c
**************************************************************************** */

void	free_tab(char **tab);
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
								  heredoc.c
**************************************************************************** */

void	handle_ctrl_d_in_heredoc(char *line, t_ast *node, t_all *all);
void	read_and_write_heredoc_lines(t_ast *node, t_all *all,
			const char *heredoc_file);
void	ft_heredoc(t_ast *node, t_all *all, int hd_index);
void	prepare_heredocs(t_all *all);
void	free_heredocs_temp(t_ast *ast);

/* ****************************************************************************
						   list_and_exit_tokenize.c
**************************************************************************** */

t_token	*new_token(char *value, t_type type, char quote_type, int i);
void	free_tokens_error(t_token *token, int need_exit);
void	free_token_list(t_token *token);
void	clean_all_and_exit(t_env *env, t_token *tokens);
void	add_token_back(t_token **token, t_token *new, t_env *env);

/* ****************************************************************************
							   main_functions.c
**************************************************************************** */

char	*handle_noninteractive_input(void);
void	exit_if_no_line(t_all *all, char *line);
t_all	*init_all(int argc, char **argv, char **envp);
void	exec_lines(t_all *all, char *line);

/* ****************************************************************************
							parse_token_for_ast.c
**************************************************************************** */

int		handle_redirection(t_token **tokens, t_ast **current_cmd, t_all *all);
int		handle_command(t_token **tokens, t_ast **current_cmd, t_all *all);
void	fill_redirection(t_ast *ast, t_token *redir, char *filename);
int		pipe_in_first_pos(t_token *tokens, t_all *all);
int		nothing_after_pipe(t_token *tokens, t_all *all);

/* ****************************************************************************
								 restore_fds.c
**************************************************************************** */

int		return_error_restore_fds(t_all *all, int stdin_tmp, int stdout_tmp);
void	restore_fds(int stdin_tmp, int stdout_tmp);
void	wait_all_children(t_all *all, pid_t last_pid);

/* ****************************************************************************
								  tokenize1.c
**************************************************************************** */

int		handle_operator(const char *line, int i, t_token **tokens, t_env *env);
int		get_word_length(const char *line, int start);
void	replace_env_key(char *env_key, t_token *token, t_all *all);
int		fill_value_env(t_token *tokens, char *line, int posi, t_all *all);
void	append_char_to_token(t_token *tokens, char c);

/* ****************************************************************************
								  tokenize2.c
**************************************************************************** */

void	handle_dollar(char *line, int *read_pos, t_token *tokens, t_all *all);
void	fill_tok_between_quotes(char *line, int *read_pos,
			t_token *tokens, t_all *all);
int		add_key_value(char *line, int ipos, t_token *tokens, t_all *all);
int		parse_word(char *line, int *read_pos, t_token *token, t_all *all);
int		handle_word(char *line, int i, t_token **tokens, t_all *all);

/* ****************************************************************************
								utils_builtins.c
**************************************************************************** */

char	*get_env_value(t_env *env, const char *key);
void	swap_env_values(t_env *a, t_env *b);
void	sort_env_list(t_env *env);
int		export_sorted(t_env *env);

/* ****************************************************************************
							utils_external.c
**************************************************************************** */

void	exec_child_process(char *cmd_path, t_ast *ast, char **envp);
void	wait_child_status(t_all *all, int pid);

/* ****************************************************************************
									utils.c
**************************************************************************** */

char	*ft_strjoin_free_all(char *s1, char *s2);
char	*ft_strjoin_free_s1(char *s1, char *s2);
int		is_space(char c);
int		is_operator_char(char c);

/* ****************************************************************************
								   builtins
**************************************************************************** */

int		cd_builtin(t_ast *node, t_all *all);
int		update_env_var(t_env *env, const char *key, char *new_value);
int		change_oldpwd(t_all *all);
int		change_pwd(t_all *all);
char	*ft_dir_to_reach(t_all *all, char *cmd);

int		echo_builtin(t_ast *node, t_all *all);
int		not_n(char *str);

int		env_builtin(t_ast *node, t_all *all);

int		exit_builtin(t_ast *node, t_all *all);
int		ft_is_numeric(const char *str);
void	print_msg_exit_not_numeric(char *str);
void	free_env_and_ast_in_builtin_exit(t_all *all);

int		export_builtin(t_ast *node, t_all *all);
int		is_valid_identifier(const char *str);
int		validate_and_split_env_var(t_all *all, char *arg,
			char **env_keyname, char **value);
int		update_or_add_env_var(t_env *env, char *env_keyname, char *value);
void	add_new_env_var(t_env **env, char *env_keyname, char *value);

int		pwd_builtin(t_all *all);

int		unset_builtin(t_ast *node, t_all *all);
void	remove_node_env(t_env **env, char **args, int i);

#endif
