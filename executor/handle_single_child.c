/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:13:22 by asebban           #+#    #+#             */
/*   Updated: 2025/04/28 12:32:43 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void    handle_single_child(t_shell *shell)
// {
//     char *path;
//     char **env_array;
//     int saved_stdin;
//     int saved_stdout;

//     saved_stdin = dup(STDIN_FILENO);
//     saved_stdout = dup(STDOUT_FILENO);
//     if (!handle_redirections_single_child(shell))
//         exit(EXIT_FAILURE);
//     path = get_path(shell, true);
//         if(!path)
//             exit(127); //command not found;
//     env_array = transform_environ_array(shell);
//     if (!env_array)
//     {
//         // //free(path);
//         exit(126); // Cannot create environment
//     }
//     execve(path, shell->executor->execs, env_array);//exit
//         // Only reached if execve fails
//         ///in fails:
//     perror("minishell"); 
//     // check_errno(shell, path);
//     // //free(path);
//     // free_str_arr(env_array);
//     dup2(saved_stdin, STDIN_FILENO);
//     dup2(saved_stdout, STDOUT_FILENO);
//     exit(126); // Execution failed
// }


void handle_single_child(t_shell *shell)
{
    char *path;
    char **env_array;
    int saved_stdin;
    int saved_stdout;
    struct stat path_stat;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);

    if (!handle_redirections_single_child(shell))
        exit(EXIT_FAILURE);

    path = get_path(shell, true);
    if (!path)
        exit(127); // Command not found

    // Use stat to check if path is a directory
    if (stat(path, &path_stat) == 0) {
        if ((path_stat.st_mode & S_IFDIR) == S_IFDIR) {
            write(STDERR_FILENO, "minishell: ", 11);
            write(STDERR_FILENO, path, strlen(path));
            write(STDERR_FILENO, ": Is a directory\n", 17);// ft putstr in 2
            exit(126); // Path is a directory, not executable
        }
    }

    env_array = transform_environ_array(shell);
    if (!env_array) {
        exit(126); // Cannot create environment
    }

    execve(path, shell->executor->execs, env_array); // Exit if successful

    // Only reached if execve fails
    perror("minishell");
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    exit(126); // Execution failed
}
