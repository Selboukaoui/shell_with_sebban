/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:02:47 by asebban           #+#    #+#             */
/*   Updated: 2025/04/16 14:02:47 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdbool.h>

# define PARENT 1
# define CHILD  2
# define IGNORE 3

typedef struct s_shell
{
    char                    *rl_input;
    char                    *rl_copy;
    struct s_environ_list   *environ;
    struct s_lexer_list     *lex_head;
    struct s_executor       *executor;
}               t_shell;

typedef struct s_environ_list
{
    struct s_environ_node       *head;
    int                         size;
}               t_environ_list;

typedef struct s_environ_node
{
    char                    *key;
    char                    *value;
    struct s_environ_node   *next;
}               t_environ_node;

typedef enum s_token
{
    PIPE,
	CMD,
	DOLLAR,
	REDERECT_IN,
	REDERECT_OUT,
	APPEND,
	HEREDOC,
	SINGLE_Q,
	DOUBLE_Q
}           t_token;

typedef struct s_lexer_list
{
    char                *str;
    t_token             type;
    struct s_lexer_list *prev;
    struct s_lexer_list *next;
}   t_lexer_list;
 
// typedef struct s_executor
// {
//     int                 id;
//     int                 size;
//     int                 fd_in;
//     int                 fd_out;
//     bool                truncate;
//     bool                append;
//     bool                redirect_input;
//     bool                heredoc;
//     char                **path;
//     char                **execs;
//     struct s_executor   *prev;
//     struct s_executor   *next;
// }               t_executor;
 
// typedef struct s_expander
// {
//     char        *beginning;
//     char        *res;
//     char        *invalid;
//     char        *env_val;
//     char        *var;
//     char        *str;
//     char        *conv;
//     int         pos;
//     int         len;
//     int         recently_invalid;
//     int         expanded;
//     int         flag;
// }               t_expander;
 
// typedef struct s_info
// {
//     int                     wstatus;
//     t_shell                 *shell;
//     int                     stdin_copy;
//     pid_t                   *pids;
// }   t_info;
 
#endif