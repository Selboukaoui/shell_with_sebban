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
# define MINISHELL_H

# include <errno.h>
# include <linux/limits.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/stat.h>
# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include "struct.h"
# include "execution.h"
# include "parsing.h"
# include "builtin.h"

# define FAILED 102
# define OK 100
# define EXIT_GET 0
# define EXIT_SET 1

# define FAIL_SYSCALL       -1
# define OKAY               0
# define FAIL_SYSCALL_PARENT    1
# define FAIL_SYSCALL_CHILD  2

# define FAIL -1
# define PARENT 1
# define CHILD  3
// # define IGNORE 3

/* expansion/splitting */
int             should_split_token(t_lexer_list *token);
void            split_expanded(t_lexer_list *token);

/* value lookup */
char           *get_value(t_shell *obj, char *str);
char           *get_special_value(t_shell *obj, char *str);

/* token creation */
t_lexer_list   *create_token(char *str);

/* main expand pass */
void            expand(t_shell *obj);

/* quote processing */
char           *no_quotes(t_shell *obj, char **argv, int *i, int *j);
char           *do_quotes(t_shell *obj, char **argv, int *i, int *j);
char           *si_quotes(char **argv, int *i);

/* token parsing */
void            process_quotes(t_shell *obj,
                               t_lexer_list *token,
                               char **argv,
                               int *ij);
void            process_token(t_shell *obj,
                              t_lexer_list *token,
                              int *j);

/* here-doc / special tokens */
void            handle_special_tokens(t_lexer_list **token_ptr);
char *handle_special_chars(t_shell *obj, char *str);

#endif