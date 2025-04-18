/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 20:07:22 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/17 20:07:22 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdbool.h>
# include <limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
# include "../libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>

# define FAILED     102
# define OK         100

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
	EMPTY	= 0,
	CMD	= 1,
	REDERECT_IN = 2, // <
	REDERECT_OUT = 3, // >
	APPEND = 4, // >>
	HEREDOC = 5, // <<
	PIPE	= 6, // |
}           t_token;

typedef struct s_lexer_list
{
    char                *str;
    t_token             type;
    struct s_lexer_list *prev;
    struct s_lexer_list *next;
}   t_lexer_list;

typedef struct s_executor
{
    int                 id;
    int                 size;
    int                 fd_in;
    int                 fd_out;
    bool                REDERECT_OUT;
    bool                append;
    bool                redirect_input;
    bool                heredoc;
    char                **path;
    char                **execs;
    struct s_executor   *prev;
    struct s_executor   *next;
}               t_executor;
 
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


t_environ_node *create_environ_node(char *var);
t_environ_node *add_back_environ_node(t_environ_list *environ, t_environ_node *node);
void free_environ(t_environ_list *environ);
t_environ_list *init_environ(char **envp);
t_environ_list *empty_environ(t_environ_list *environ);
t_shell *init_shell(char **env);
char *clean_rl_copy(char *rl_copy);
int check_quote_syntax(const char *input);
bool    parser(t_shell *shell);
t_lexer_list *tokenize(t_shell *shell);
char	**ft_newsplit(const char *s);
char *free_str_arr(char **arr);
t_executor *prepare_executor(t_shell *shell);
t_executor *fill_executor_list(t_shell *shell, t_executor *list);
void set_path_executor(t_executor *list, t_environ_list *environ);
t_environ_node *get_node(t_environ_list *environ, char *key);


int ft_strcmp(char *s1, char *s2);

// syntax checking functions
void    signal_setup(int mode);
int check_parenthesis(char *input);
int check_redirect_out(char *input);
int check_redirect_in(char *input);

char *ft_strcpy(char *dest, const char *src);

// t_lexer_list *tokenize_and_split(t_shell *shell);

t_token which_type(char *s);
int check_pipe(char *input);
char **transform_environ_array(t_shell *shell);

char *get_path(t_shell *shell, bool printerror);

void    handle_single_child(t_shell *shell);

t_executor *free_executor_list(t_executor *list);

void executor(t_shell *shell);

























#endif