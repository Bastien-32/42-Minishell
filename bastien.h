#ifndef BASTIEN_H
# define BASTIEN_H

# include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	struct s_token	*next;
}					t_token;

# endif