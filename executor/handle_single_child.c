/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:13:22 by asebban           #+#    #+#             */
/*   Updated: 2025/04/18 12:25:31 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    handle_single_child(t_shell *shell)
{
    char *path;
    char *env_array;
    int saved_stdin;
    int saved_stdout;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    // if (!handle_redirections_single_child(shell))
    //     exit(EXIT_FAILURE);
    // path = get_path(shell, true);
}