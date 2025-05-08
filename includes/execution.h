/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:32:10 by asebban           #+#    #+#             */
/*   Updated: 2025/05/08 17:35:51 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

void			wait_pipeline(t_info *info, int number);
t_environ_node	*create_environ_node(char *var);
t_environ_node	*add_back_environ_node(t_environ_list *env,
					t_environ_node *node);
t_environ_list	*init_environ(char **envp);
t_environ_list	*empty_environ(t_environ_list *env);
t_shell			*init_shell(char **env);
t_executor		*prepare_executor(t_shell *shell);
t_executor		*fill_executor_list(t_shell *shell, t_executor *list);
void			set_path_executor(t_executor *list, t_environ_list *env);
t_environ_node	*get_node(t_environ_list *env, char *key);
t_info			*init_info(t_shell *shell);
int				child_handler_multi(int *fildes,
					t_executor *current, t_info *info);
void			handle_multi(t_info *info, t_executor *current);
t_info			*init_info(t_shell *shell);
int				check_pipe(char *input);
char			**transform_environ_array(t_shell *shell);
char			*get_path(t_shell *shell, bool printerror);
void			handle_single_child(t_shell *shell);
void			executor(t_shell *shell);
bool			handle_redirections_single_child(t_shell *shell);
char			*get_env_value(t_environ_list *env_list, char *key);
t_environ_node	*ft_getenv(t_environ_list *env, char *key);
int				is_cmdline_empty(const char *cmdline);
void			try_exec_with_fallback(char *path,
					char **args, char **envp, t_shell *shell);
char			*execute_other_helper(t_executor *current);
void			err_for_norm(char *cmd);
void			get_path_error(char *str);
#endif