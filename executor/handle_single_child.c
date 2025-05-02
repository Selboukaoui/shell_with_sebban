/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:13:22 by asebban           #+#    #+#             */
/*   Updated: 2025/05/02 12:06:34 by asebban          ###   ########.fr       */
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


// void handle_single_child(t_shell *shell)
// {
//     char *path;
//     char **env_array;
//     int saved_stdin;
//     int saved_stdout;
//     struct stat path_stat;

//     saved_stdin = dup(STDIN_FILENO);
//     saved_stdout = dup(STDOUT_FILENO);

//     if (!handle_redirections_single_child(shell))
//         exit(EXIT_FAILURE);

//     path = get_path(shell, true);
//     if (!path)
//         exit(127); // Command not found

//     // Use stat to check if path is a directory
//     if (stat(path, &path_stat) == 0) {
//         if ((path_stat.st_mode & S_IFDIR) == S_IFDIR) {
//             write(STDERR_FILENO, "minishell: ", 11);
//             write(STDERR_FILENO, path, strlen(path));
//             write(STDERR_FILENO, ": Is a directory\n", 17);// ft putstr in 2
//             exit(126); // Path is a directory, not executable
//         }
//     }

//     env_array = transform_environ_array(shell);
//     if (!env_array) {
//         exit(126); // Cannot create environment
//     }

//     execve(path, shell->executor->execs, env_array); // Exit if successful

//     // Only reached if execve fails
//     perror("minishell");
//     dup2(saved_stdin, STDIN_FILENO);
//     dup2(saved_stdout, STDOUT_FILENO);
//     exit(126); // Execution failed
// }

#include "../includes/minishell.h"

void handle_single_child(t_shell *shell)
{
    char        *path;
    char        **env_array;
    int          saved_stdin;
    int          saved_stdout;
    struct stat  st;
    char        *cmd = shell->executor->execs[0];

    saved_stdin  = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);

    if (!handle_redirections_single_child(shell))
        exit(EXIT_FAILURE);

    if (is_cmdline_empty(cmd))
    {
            // skip parsing
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        exit(127);
        
    }
    /* If user typed a slash anywhere (./foo or /usr/bin/foo), use it literally.
     * Otherwise, search PATH—but since get_path now skips “.”, 
     * bare filenames in CWD won’t be found. */
    if (strchr(cmd, '/'))
        path = cmd;
    else
        path = get_path(shell, true);

    /* Not found? */
    if (!path || stat(path, &st) != 0)
    {
        write(STDERR_FILENO, "minishell: command not found: ", 30);
        write(STDERR_FILENO, cmd, strlen(cmd));
        write(STDERR_FILENO, "\n", 1);
        exit(127);
    }

    /* Directory? */
    if ((st.st_mode & S_IFDIR) == S_IFDIR)
    {
        write(STDERR_FILENO, "minishell: ", 11);
        write(STDERR_FILENO, path, strlen(path));
        write(STDERR_FILENO, ": Is a directory\n", 17);
        exit(126);
    }

    env_array = transform_environ_array(shell);
    if (!env_array)
        exit(126);

    execve(path, shell->executor->execs, env_array);

    /* execve failed */
    perror("minishell");
    dup2(saved_stdin,  STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    exit(126);
}
