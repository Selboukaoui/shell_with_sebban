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


#include <errno.h>
#include <linux/limits.h>
#include <stdbool.h>
# include <limits.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
# include "../libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
// #include <linux/limits.h>

static int g_signals;
# define FAILED     102
# define OK         100

//exit
#define EXIT_GET 0
#define EXIT_SET 1

//  handle_multi()
# define FAIL_SYSCALL       -1
# define OKAY               0
# define FAIL_SYSCALL_PARENT    1
# define FAIL_SYSCALL_CHILD  2

# define PARENT 1
# define CHILD  2
# define IGNORE 3

typedef struct s_shell
{
    char                    *rl_input;
    char                    *rl_copy;
    struct s_environ_list   *env;
    struct s_lexer_list     *lex_head;
    struct s_executor       *executor;
}               t_shell;

typedef struct s_environ_list
{
    struct s_environ_node       *head;
    // bool                        IS_NUUL;
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
 
typedef struct s_info
{
    int                     wstatus;
    t_shell                 *shell;
    int                     stdin_copy;
    pid_t                   *pids;
}   t_info;


typedef struct garbage_c
{
	void				*data;
	struct garbage_c	*next;
}	t_collect;

// typedef struct s_mini_garbage
// {
// 	void				*data;
// 	struct s_mini_garbage	*next;
// }	t_alloc;
// t_lexer_list *tokenize(t_shell *shell);

t_lexer_list *tokenize(t_shell *shell);
t_lexer_list    *lexer(t_shell *shell);
// char *ft_strndup(const char *s, size_t n);
// void	*ft_mini_g(size_t size, int mode);
char *save_pwd(int action, char *value);
int	unset(t_shell *shell, char **args);
void wait_pipeline(t_info *info, int number);
t_environ_node *create_environ_node(char *var);
t_environ_node *add_back_environ_node(t_environ_list *env, t_environ_node *node);
// void free_enviro(t_environ_list *env);
t_environ_list *init_environ(char **envp);
t_environ_list *empty_environ(t_environ_list *env);
t_shell *init_shell(char **env);
char *clean_rl_copy(char *rl_copy);
int check_quote_syntax(const char *input);
bool    parser(t_shell *shell);
char	**ft_newsplit(const char *s);
// char *free_str_arr(char **arr);
t_executor *prepare_executor(t_shell *shell);
t_executor *fill_executor_list(t_shell *shell, t_executor *list);
void set_path_executor(t_executor *list, t_environ_list *env);
t_environ_node *get_node(t_environ_list *env, char *key);
t_info *init_info(t_shell *shell);
int child_handler_multi(int *fildes, t_executor *current, t_info *info);
void handle_multi(t_info *info, t_executor *current);
int ft_strcmp(const char *s1, const char *s2);
t_info *init_info(t_shell *shell);
// syntax checking functions
void    signal_setup(int mode);
int check_parenthesis(char *input);
int check_redirect_out(char *input);
int check_redirect_in(char *input);
void int_to_str(int num, char *str);
char *ft_strcpy(char *dest, const char *src);
int	echo(char **args);
// t_lexer_list *tokenize_and_split(t_shell *shell);
char *ft_strncpy(char *dest, const char *src, size_t n);
t_token which_type(char *s, int inside);

int check_pipe(char *input);
char **transform_environ_array(t_shell *shell);

char *get_path(t_shell *shell, bool printerror);

void    handle_single_child(t_shell *shell);

// t_executor *free_executor_list(t_executor *list);

void executor(t_shell *shell);


bool handle_redirections_single_child(t_shell *shell);

int handle_single_builtin(t_shell *shell);

bool    is_builtin(char *exec);
int 	pwd (t_environ_list *env);
int 	cd (t_shell *shell, char **arg);

void	*ft_malloc(size_t size, int mode);

int env(t_shell *shell);

int exit_status(int action, int value);


char *replace_vars(char *input, t_shell *shell);


int export(t_shell *shell, char **args);

// char *get_env_value(t_shell *shell, const char *key);

char *get_env_value(t_environ_list *env_list, char *key);
t_environ_node   *ft_getenv(t_environ_list *env, char *key);
int exit_builtin(t_shell *shell, char **args, int in_pipe);
int is_cmdline_empty(const char *cmdline);


void free_enviro(t_shell *shell);
#endif     