/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:13:22 by asebban           #+#    #+#             */
/*   Updated: 2025/05/04 11:09:21 by selbouka         ###   ########.fr       */
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

// void handle_single_child(t_shell *shell)
// {
//     char        *path;
//     char        **env_array;
//     int          saved_stdin;
//     int          saved_stdout;
//     struct stat  st;
//     char        *cmd = shell->executor->execs[0];

//     saved_stdin  = dup(STDIN_FILENO);
//     saved_stdout = dup(STDOUT_FILENO);

//     if (!handle_redirections_single_child(shell))
//         exit(EXIT_FAILURE);

//     if (is_cmdline_empty(cmd))
//     {
//             // skip parsing
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         ft_putstr_fd(cmd, STDERR_FILENO);
//         ft_putstr_fd(": command not found\n", STDERR_FILENO);
//         exit(127);
        
//     }
//     /* If user typed a slash anywhere (./foo or /usr/bin/foo), use it literally.
//      * Otherwise, search PATH—but since get_path now skips “.”, 
//      * bare filenames in CWD won’t be found. */
//     if (strchr(cmd, '/'))
//         path = cmd;
//     else
//         path = get_path(shell, true);

//     /* Not found? */
//     // if (!path)
//     // {
        
//     // }
//     if (!path || stat(path, &st) != 0)
//     {
//         write(STDERR_FILENO, "minishell: command not found: ", 30);
//         write(STDERR_FILENO, cmd, strlen(cmd));
//         write(STDERR_FILENO, "\n", 1);
//         exit(127);
//     }

//     /* Directory? */
//     if ((st.st_mode & S_IFDIR) == S_IFDIR)
//     {
//         write(STDERR_FILENO, "minishell: ", 11);
//         write(STDERR_FILENO, path, strlen(path));
//         write(STDERR_FILENO, ": Is a directory\n", 17);
//         exit(126);
//     }

//     env_array = transform_environ_array(shell);
//     if (!env_array)
//         exit(126);

//     execve(path, shell->executor->execs, env_array);

//     /* execve failed */
//     perror("minishell");
//     dup2(saved_stdin,  STDIN_FILENO);
//     dup2(saved_stdout, STDOUT_FILENO);
//     exit(126);
// }
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void try_exec_with_fallback(char *path, char **args, char **envp)
{
    execve(path, args, envp);

    // If execve fails
    // perror("execve failed"); // Optional: print error
    if (errno == ENOENT || errno == EACCES || errno == ENOEXEC)
    {
        // Build the fallback command string
        // For example: sh -c "ls -la"
        size_t total_len = 0;
        for (int i = 0; args[i]; i++)
            total_len += strlen(args[i]) + 1;

        char *cmd = ft_malloc(total_len + 1, 1);
        if (!cmd)
            exit(1);

        cmd[0] = '\0';
        for (int i = 0; args[i]; i++)
        {
            strcat(cmd, args[i]);
            if (args[i + 1])
                strcat(cmd, " ");
        }

        // Now execute: /bin/sh -c "your command string"
        char *sh_args[] = {"/bin/sh", "-c", cmd, NULL};
        execve("/bin/sh", sh_args, envp);
        perror("fallback execve failed");
        free(cmd);
        exit(127); // fallback also failed
    }

    exit(126); // original execve failed for other reasons
}

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
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        exit(127);
    }

    if (strchr(cmd, '/'))
    {
        // Literal path
        path = cmd;
    }
    else
    {
        // 1) Try to resolve via PATH in shell->env
        path = get_path(shell, true);

        // 2) If that failed, check if our shell’s PATH is unset/empty
        if (!path)
        {
            char *path_env = get_env_value(shell->env, "PATH");
            if (!path_env || *path_env == '\0')
            {
                // build "./cmd"
                size_t len = strlen(cmd) + 3;
                path = ft_malloc(len, 1);
                if (!path)
                    exit(126);
                ft_memcpy(path, "./", 2);
                ft_memcpy(path + 2, cmd, strlen(cmd) + 1);
            }
        }

        // 3) If still no path, error out
        if (!path)
        {
            ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
            ft_putstr_fd(cmd, STDERR_FILENO);
            ft_putstr_fd("\n", STDERR_FILENO);
            exit(127);
        }
    }

    // Now verify it exists
    if (stat(path, &st) != 0)
    {
        ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
        ft_putstr_fd(cmd, STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        if (path != cmd) free(path);
        exit(127);
    }

    // Directory?
    if (S_ISDIR(st.st_mode))
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
        if (path != cmd) free(path);
        exit(126);
    }

    env_array = transform_environ_array(shell);
    if (!env_array)
    {
        if (path != cmd) free(path);
        exit(126);
    }

    execve(path, shell->executor->execs, env_array);
    try_exec_with_fallback(path, shell->executor->execs, env_array);
    // execve failed
    perror("minishell");
    dup2(saved_stdin,  STDIN_FILENO);//dup in try
    dup2(saved_stdout, STDOUT_FILENO);
    if (path != cmd) free(path);
    exit(126);
}
