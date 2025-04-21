/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:13:22 by asebban           #+#    #+#             */
/*   Updated: 2025/04/21 15:24:47 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    handle_single_child(t_shell *shell)
{
    char *path;
    char **env_array;
    int saved_stdin;
    int saved_stdout;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (!handle_redirections_single_child(shell))
        exit(EXIT_FAILURE);
    path = get_path(shell, true);
        if(!path)
            exit(127); //command not found;
    env_array = transform_environ_array(shell);
    if (!env_array)
    {
        // //free(path);
        exit(126); // Cannot create environment
    }
    execve(path, shell->executor->execs, env_array);
        // Only reached if execve fails

    ///in fails:
    // check_errno(shell, path);
    // //free(path);
    // free_str_arr(env_array);
    // dup2(saved_stdin, STDIN_FILENO);
    // dup2(saved_stdout, STDOUT_FILENO);
    // exit(126); // Execution failed
}