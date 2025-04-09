#ifndef MINISHELL_PARS_H
#define MINISHELL_PARS_H

#include <stdio.h>
#include <signal.h>


typedef enum e_lexer_state
{
	S_NORMAL,
	S_DOUBLE_Q,
	S_SINGLE_Q,
	S_VARIABLE
} t_state;

typedef enum e_type_of_the_input
{
	PIPE,
	WORD,
	DOLLAR,
	REDERECT_IN,
	REDERECT_OUT,
	APPEND,
	HEREDOC,
	SINGLE_Q,
	DOUBLE_Q
} t_TokenType;

typedef struct s_token
{
	char    *value;
	t_TokenType type;
	s_token	*next;
} t_token;

#endif