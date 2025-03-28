#ifndef BASTIEN_H
# define BASTIEN_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>

# include "libft.h"

typedef enum	e_token_type
{
	TOKEN_WORD,			// ex: ls, hello, input.txt
	TOKEN_PIPE,			// |
	TOKEN_REDIR_IN,		// <
	TOKEN_REDIR_OUT,	// >
	TOKEN_APPEND,		// >>
	TOKEN_HEREDOC		// <<
}	t_token_type;

typedef struct	s_token
{
	char			*value;
	t_token_type	type;
	char			quote_type;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*env_keyname;
	char			*value;
	struct s_env	*next;
}					t_env;

/* ****************************************************************************
									env.c
**************************************************************************** */

t_env	*create_env_node(const char *env_keyname, const char *value);
void	add_env_back(t_env **env, t_env *new);
t_env	*init_env(char **envp);
void	free_env_error(t_env *env, int need_exit);
void	free_env_list(t_env *env);

/* ****************************************************************************
							expand_token_values_b.c
**************************************************************************** */

int		env_vars(char *value_token);
char	*add_key_value(char *val_tok, int *read_pos, char *new_tok, t_env *env);
char	*fill_value_env(char *value_token, t_env *env);
void	expand_token_values(t_token *tokens, t_env *env);

/* ****************************************************************************
							expand_token_values_b.c
**************************************************************************** */

t_token	*new_token(const char *value, t_token_type type, char quote_type);
void	free_tokens_error(t_token *token, int need_exit);
void	free_token_list(t_token *token);
void	clean_all_and_exit(t_env *env, t_token *tokens);
void	add_token(t_token **token, t_token *new, t_env *env);
int		is_space(char c);
int		is_operator_char(char c);
int		handle_operator(const char *line, int i, t_token **tokens, t_env *env);
int		get_word_length(const char *line, int start, t_token *tokens, t_env *env);
char	*clean_quotes(const char *str,  t_token *tokens, t_env *env);
char	fill_quote_type(const char *str);
int		handle_word(const char *line, int i, t_token **tokens, t_env *env);
t_token	*tokenize(const char *line, t_env *env);

/* ****************************************************************************
									utils.c
**************************************************************************** */

char	*ft_strjoin_free_all(char *s1, char *s2);
char	*ft_strjoin_free_s1(char *s1, char *s2);

# endif