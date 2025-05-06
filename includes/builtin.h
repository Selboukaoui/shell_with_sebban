/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:45:14 by asebban           #+#    #+#             */
/*   Updated: 2025/05/06 18:04:50 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"

char	*save_pwd(int action, char *value);
int		unset(t_shell *shell, char **args);
int		echo(char **args);
int		handle_single_builtin(t_shell *shell);
bool	is_builtin(char *exec);
int		pwd(t_environ_list *env);
int		cd(t_shell *shell, char **arg);
void	*ft_malloc(size_t size, int mode);
int		exit_builtin(t_shell *shell, char **args, int in_pipe);
int		env(t_shell *shell);
int		export(t_shell *shell, char **args);
int		exit_status(int action, int value);
#endif