/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:11:51 by asebban           #+#    #+#             */
/*   Updated: 2025/04/21 14:20:16 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void clean_exit(t_shell *shell, int exit_code)
{
    //clean
    exit(exit_code);
}

int exit_builtin(t_shell *shell, char **args, bool in_child)
{
    int exit_code;
    int arg_count;

    arg_count = 0;
    while (args[arg_count])
        arg_count++;
    if (arg_count == 0)
        clean_exit(shell, exit_code);
}