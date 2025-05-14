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
	COMMAND,	// ex: ls, hello, input.txt
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
}					t_token;

typedef struct s_env
{
	char			*env_keyname;
	char			*value;
	//int				nb_env;
	struct s_env	*next;
}					t_env;

/* typedef struct s_ast
{
	t_type			type;
	char			**cmd;			// ex: ["echo", "salut", NULL]
	char			*filename;		// utilisé si type == REDIR_xx
	int				fd;				// facultatif pour la redirection
	struct s_ast	*left;
	struct s_ast	*right;
	int				visited;
}	t_ast; */

typedef struct s_ast
{
	char			**cmd;			// Tableau de la commande + arguments (ex: {"grep", "bonjour", NULL})
	char			*redir_in;		// Fichier pour < ou <<
	char			*redir_out;		// Fichier pour > ou >> 
	t_type			type_in;		// type de l'entree (REDIR_IN ou HEREDOC)
	t_type			type_out;		// type de la sortie (REDIR_OUT ou  APPEND)
	int				pipe_out;		// true si un pipe | suit cette commande
	struct s_ast	*next;			// Commande suivante (chaînée)
}	t_ast;

typedef struct s_all {
	t_env	*env;
	t_ast	*ast;
	int		exit_status;
	char	**lines;
}			t_all;

/* ****************************************************************************
								 execute_ast.c
**************************************************************************** */


int		is_valid_command(char **cmd);
int		execute_command(t_ast *node, t_all *all);
int		execute_single(t_ast *ast, t_all *all);
void	execute_command_child(t_ast *node, t_all *all);
int		pipe_child_process(t_ast *node, t_all *all, int fd_in, int fd_out);
int		execute_pipe(t_ast **ast_ptr, t_all *all, int *fd_in, pid_t *last_pid);
void	restore_std_and_wait_all_children(t_all *all,pid_t last_pid,
		int stdin_tmp, int stdout_tmp);
int		return_error_restore_fds(t_all *all,int stdin_tmp, int stdout_tmp);
int		execute_ast(t_all *all);

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
char	**env_to_array(t_env *env);
void	free_array_envp(char **envp);
int		prepare_env_and_path(t_all *all, t_ast *node, char **cmd_path, char ***envp);
void	exec_child_process(char *cmd_path, t_ast *ast, char **envp);
void	wait_child_status(t_all *all, int pid);
int		execute_external(t_ast *node, t_all *all);

/* ****************************************************************************
						  execute_command_external.c
**************************************************************************** */

int		perror_message(t_all *all,char *str);
int		execute_redirection(t_ast *node, t_all *all);


/* ****************************************************************************
						  execute_command_external.c
**************************************************************************** */

//int	ft_heredoc(t_ast *node, t_all *all);

/* ****************************************************************************
									ast.c
**************************************************************************** */

t_ast	*new_ast_node(char **value);
void	free_ast_error(t_ast *ast);
int		clean_ast_and_exit(t_ast *ast, t_env *env, t_token *tokens);
int		add_back_ast(t_all *all, t_ast *new, t_token *token);
t_ast	*parse_commands_in_block(t_token **tokens);

/* ****************************************************************************
								 build_tree.c
**************************************************************************** */

t_ast	*ft_build_tree(char *line, t_all *all);

t_token	*tokenize(char *line, t_all *all);

void	expand_token_values(t_token *tokens, t_all *all);

int	count_cmd_tokens(t_token *tokens, t_ast **current_cmd, t_all *all);

void	free_cmd_args(char **args, int count);

char	**dup_cmd_tokens(t_token **tokens, int count, t_all *all);

int	handle_command(t_token **tokens, t_ast **current_cmd, t_all *all);

void	fill_redirection(t_ast *ast, t_token *redir, char *filename);

int	handle_redirection(t_token **tokens, t_ast **current_cmd, t_all *all);

int	parse_token_for_ast(t_token **tokens, t_ast **current_cmd, t_all *all);

t_ast	*parse_ast(t_token *tokens, t_all *all);

/* ****************************************************************************
									env.c
**************************************************************************** */

/**
 * @brief Creates a new environment variable node.
 *
 * Allocates and initializes a t_env structure with the given key name and 
 * value.
 *
 * @param env_keyname The name of the environment variable (e.g., "PATH").
 * @param value The value associated with the variable.
 * @return A pointer to the new node, or NULL on failure.
 */
t_env	*create_env_node(const char *env_keyname, const char *value);

/**
 * @brief Adds a node to the end of the environment list.
 *
 * Inserts a new t_env node at the end of the linked environment list.
 *
 * @param env Pointer to the start of the environment list.
 * @param new The new node to append.
 */
void	add_env_back(t_env **env, t_env *new);

/**
 * @brief Initializes the environment list from envp.
 *
 * Converts the envp array of strings into a linked list of t_env nodes.
 *
 * @param envp The environment variable array (for example: 
 * ["PATH=/bin", ...]).
 * @return A pointer to the head of the created list.
 */
t_env	*init_env(char **envp);

/**
 * @brief Frees the environment list and exits the program if needed.
 *
 * Releases all memory used by the environment list and optionally exits the 
 * program.
 *
 * @param env The environment list to free.
 * @param need_exit Set to 1 to exit the program after freeing, or 0 to 
 * continue.
 */
void	free_env_error(t_env *env, int need_exit);

/**
 * @brief Frees the entire environment list.
 *
 * Iterates through the list and frees each node and its contents.
 *
 * @param env The environment list to free.
 */
void	free_env_list(t_env *env);

/* ****************************************************************************
							expand_token_values_b.c
**************************************************************************** */

/**
 * @brief Checks if a token contains a valid environment variable reference.
 *
 * Scans the given token for the presence of a '$' character followed by a valid
 * character (not a space), indicating an environment variable to expand.
 *
 * @param value_token The token value to inspect.
 * @return 1 if a valid environment variable reference is found, 0 otherwise.
 */
int		env_vars(char *value_token);

int	add_key_value(char *line, int ipos, t_token *tokens, t_all *all);

/**
 * @brief Expands all environment variables in a given token value.
 *
 * Replaces all '$KEY' patterns with their corresponding values from the 
 * environment.
 *
 * @param value_token The token string possibly containing variables.
 * @param env The environment used for variable resolution.
 * @return A new string with variables expanded, or NULL on error.
 */
char	*fill_value_env(char *value_token, t_env *env);

/* ****************************************************************************
						   list_and_exit_tokenize.c
**************************************************************************** */

/**
 * @brief Creates a new token.
 *
 * Allocates and initializes a new token with the given value, type, and quote type.
 *
 * @param value The token string value.
 * @param type The type of the token.
 * @param quote_type The type of quote used ('\'', '"', or 0).
 * @return A pointer to the new token or NULL on failure.
 */
t_token	*new_token(char *value, t_type type, char quote_type);

/**
 * @brief Frees a token and exits optionally.
 *
 * Frees a single token or token chain and exits the program if need_exit is true.
 *
 * @param token The token(s) to free.
 * @param need_exit If true, exits the program after freeing.
 */
void	free_tokens_error(t_token *token, int need_exit);

/**
 * @brief Frees a full token list.
 *
 * Deallocates all memory used by a token linked list.
 *
 * @param token Pointer to the first token in the list.
 */
void	free_token_list(t_token *token);

/**
 * @brief Frees environment and token list, then exits.
 *
 * Cleans all allocated memory and exits the program.
 *
 * @param env Pointer to the environment structure.
 * @param tokens Pointer to the first token in the list.
 */
void	clean_all_and_exit(t_env *env, t_token *tokens);

/**
 * @brief Adds a token to the token list.
 *
 * Appends a new token to the end of an existing list. Frees everything and 
 * exits on error.
 *
 * @param token Pointer to the head of the token list.
 * @param new The new token to add.
 * @param env Pointer to the environment (used for cleanup on failure).
 */
void	add_token_back(t_token **token, t_token *new, t_env *env);

/* ****************************************************************************
								  tokenize.c
**************************************************************************** */

/**
 * @brief Identifies an operator in the input and adds it to the token list.
 *
 * Parses the command line starting from a given character, and if an operator
 *  is found (like |, >, <<, etc.), it is identified and added to the token 
 * list.
 *
 * @param line The input line.
 * @param i The current index in the line.
 * @param tokens Pointer to the token list to add to.
 * @param env Pointer to the environment (for cleanup on failure).
 * @return The updated index after the operator.
 */
int		handle_operator(const char *line, int i, t_token **tokens, t_env *env);

/**
 * @brief Calculates the length of a word starting at a given index.
 *
 * Determines how many characters make up the next word in the input.
 *
 * @param line The input line.
 * @param start Starting index of the word.
 * @param tokens Current token list.
 * @param env Pointer to the environment.
 * @return The length of the word.
 */
int		get_word_length(const char *line, int start);

/**
 * @brief Removes quotes from a string.
 *
 * Returns a new string without surrounding quotes, handling escape cases if needed.
 *
 * @param str The quoted string.
 * @param tokens Current token list (used for cleanup on error).
 * @param env Pointer to the environment.
 * @return New unquoted string or NULL on failure.
 */
char	*clean_quotes(const char *str, t_token *tokens, t_env *env);

/**
 * @brief Detects the quote type of a string.
 *
 * Returns the type of quote used to surround the string.
 *
 * @param str The input string.
 * @return The quote character used ('\'' or '"'), or 0 if none.
 */
char	fill_quote_type(const char *str);

int	handle_word(char *line, int i, t_token **tokens, t_all *all);


/* ****************************************************************************
									utils.c
**************************************************************************** */

/**
 * @brief Joins two strings and frees both.
 *
 * Concatenates s1 and s2 into a new string, frees both inputs, and returns the result.
 *
 * @param s1 First string (freed).
 * @param s2 Second string (freed).
 * @return New string or NULL on failure.
 */
char	*ft_strjoin_free_all(char *s1, char *s2);

/**
 * @brief Concatenates s1 and s2 into a new string, frees s1, and returns the
 *  result.
 *
 * @param s1 First string (freed).
 * @param s2 Second string.
 * @return New string or NULL on failure.
 */
char	*ft_strjoin_free_s1(char *s1, char *s2);

/**
 * @brief Checks if a character is a whitespace character.
 *
 * Recognizes standard whitespace characters commonly used in shell parsing:
 * 
 * - `'\t'` (horizontal tab)
 * 
 * - `'\\n'` (newline)
 * 
 * - `'\v'` (vertical tab)
 * 
 * - `'\f'` (form feed)
 * 
 * - `'\r'` (carriage return)
 * 
 * - `' '` (space)
 * 
 * @param c The character to check.
 * @return 1 if the character is a space or tab, 0 otherwise.
 */
int		is_space(char c);

/**
 * @brief Checks if a character is a valid shell operator.
 *
 * Recognized operator characters:
 *
 * - `'|'`  (pipe)
 * 
 * - `'<'`  (input redirection)
 * 
 * - `'>'`  (output redirection)
 *
 * - `'<<'` (heredoc)
 * 
 * - `'>>'` (append)
 *
 * @param c The character to check.
 * @return 1 if the character is part of a valid operator, 0 otherwise.
 */
int		is_operator_char(char c);

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
