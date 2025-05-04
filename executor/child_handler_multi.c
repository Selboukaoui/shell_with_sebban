/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_handler_multi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:38:38 by asebban           #+#    #+#             */
/*   Updated: 2025/05/04 12:32:15 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int handle_redirections_pipeline(int *fildes, t_executor *current, t_info *info)
// {
//     if (current->fd_in != STDIN_FILENO)
//     {
//         if (dup2(current->fd_in, STDIN_FILENO) == -1)
//             return (FAIL_SYSCALL);
//         close(current->fd_in);
//     }
//     else if (info->stdin_copy != STDIN_FILENO)
//     {
//         if (dup2(info->stdin_copy, STDIN_FILENO) == -1)
//             return (FAIL_SYSCALL);
//     }

//     if (current->fd_out != STDOUT_FILENO)
//     {
//         if (dup2(current->fd_out, STDOUT_FILENO) == -1)
//             return (FAIL_SYSCALL);
//         close(current->fd_out);
//     }
//     else if (fildes[1] != STDOUT_FILENO)
//     {
//         if (dup2(fildes[1], STDOUT_FILENO) == -1)
//             return (FAIL_SYSCALL);
//     }

//     close(fildes[0]);
//     close(fildes[1]);
//     return (OKAY);
// }

static int handle_redirections_pipeline(int *pipefd,
                                        t_executor *cur)
{
    // 1) If this command wants its stdin from a pipe:
    if (cur->fd_in != STDIN_FILENO)
    {
        if (dup2(cur->fd_in, STDIN_FILENO) == -1)
            return FAIL_SYSCALL;
        close(cur->fd_in);
    }

    // 2) If this command wants its stdout into a pipe:
    if (cur->fd_out != STDOUT_FILENO)
    {
        if (dup2(cur->fd_out, STDOUT_FILENO) == -1)
            return FAIL_SYSCALL;
        close(cur->fd_out);
    }

    // 3) Now that we've dup’d, close *both* ends of the pipe in the child
    //    so that the read‐end will see EOF when the writer exits.
    //    (Guard against closing std fds < 0 or < 3 if you like.)
    close(pipefd[0]);
    close(pipefd[1]);

    return OKAY;
}

// executor.c

static char *path_join(const char *dir, const char *file)
{
    char *tmp  = ft_strjoin(dir, "/");
    if (!tmp) return NULL;
    char *full = ft_strjoin(tmp, file);
    free(tmp);
    return full;
}

/**
 * Try to resolve current->execs[0]:
 * 1) if it starts with '/' or '.', treat it literally
 * 2) else scan current->path[]
 * 3) else if current->path is NULL        → PATH unset: try "./cmd"
 * Returns a ft_malloc'd path, or NULL if not found.
 * Leaves errno unchanged if “not found”; sets errno=EACCES if found but not executable.
 */
static char *execute_other_helper(t_executor *current)
{
    char    *candidate;
    struct stat st;
    int      i;

    // 1) Literal
    if (current->execs[0][0] == '/' || current->execs[0][0] == '.')
    {
        if (stat(current->execs[0], &st) == 0)
            return ft_strdup(current->execs[0]);
        return NULL;
    }

    // 2) Search PATH entries
    if (current->path)
    {
        for (i = 0; current->path[i]; i++)
        {
            candidate = path_join(current->path[i], current->execs[0]);
            if (!candidate) return NULL;
            if (stat(candidate, &st) == 0)
            {
                // Found a file—return even if not executable,
                // so execve can give EACCES.
                return candidate;
            }
            free(candidate);
        }
    }

    // 3) PATH unset in your minishell: try "./cmd"
    if (!current->path)
    {
        size_t len = 2 + strlen(current->execs[0]) + 1;
        char *dotcmd = ft_malloc(len, 1);
        if (!dotcmd) return NULL;
        memcpy(dotcmd, "./", 2);
        memcpy(dotcmd + 2, current->execs[0], strlen(current->execs[0]) + 1);
        if (stat(dotcmd, &st) == 0)
            return dotcmd;
        free(dotcmd);
    }

    return NULL;
}
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
        {
            ft_mini_g(0,0);
            ft_malloc(0,0);
            exit(1);
        }

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
int execute_other(t_executor *current, t_info *info)
{
    char       *path;
    char      **env_array;
    struct stat st;

    path = execute_other_helper(current);
    if (!path)
    {
        // not found in any of the steps
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(current->execs[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        exit(127);
    }

    // If it exists but isn't executable, execve will set errno=EACCES
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(path, STDERR_FILENO);
        ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
        free(path);
        exit(126);
    }

    env_array = transform_environ_array(info->shell);
    if (!env_array)
    {
        free(path);
        exit(126);
    }

    execve(path, current->execs, env_array);
    try_exec_with_fallback(path, current->execs,env_array);

    // execve failed—could be EACCES, ENOEXEC, etc.
    if (errno == EACCES)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(current->execs[0], STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        free(path);
        exit(126);
    }
    else
    {
        perror("minishell");
        free(path);
        exit(126);
    }
}

// static char *path_join(const char *dir, const char *file)
// {
//     char *tmp;
//     char *full;

//     // dir + "/" 
//     tmp = ft_strjoin(dir, "/");
//     if (!tmp)
//         return NULL;

//     // (dir + "/") + file
//     full = ft_strjoin(tmp, file);
//     // //free(tmp);
//     return full;
// }


//  static char *execute_other_helper(t_executor *current)
// {
//     char    *full_path;
//     int     i;

//     if (current->execs[0][0] == '/' || current->execs[0][0] == '.')
//     {
//         if (access(current->execs[0], F_OK | X_OK) == 0)
//             return (ft_strdup(current->execs[0]));
//         return (NULL);
//     }

//     i = 0;
//     // while (current->path[i])
//     // {
//     //     printf("-----------__>%s\n", current->path[i++]);
//     // }
//     // i = 0;
//     // while (current->path && current->path[i])
//     // {
//     //     full_path = ft_strjoin(current->path[i], current->execs[0]);
//     //     if (!full_path)
//     //         return (NULL);
//     //     if (access(full_path, F_OK | X_OK) == 0)
//     //         return (full_path);
//     //     //free(full_path);
//     //     i++;
//     // }
//     i = 0;
//         while (current->path && current->path[i])
//         {
//             full_path = path_join(current->path[i], current->execs[0]);
//             if (!full_path)
//                 return NULL;
//             if (access(full_path, F_OK | X_OK) == 0)
//                 return full_path;
//             // //free(full_path);
//             i++;
//         }
//     // printf("here\n");
//     return (NULL);
// }

// // char *get_path(t_shell *shell, bool printerror)
// // {
// //     char    *path;
// //     if(!shell->executor->execs || !shell->executor->execs[0])
// //         return(NULL);
// //     // If command is already an absolute path
// //     if (ft_strchr((shell->executor->execs[0]), '/'))
// //     {
// //         path = ft_strdup(shell->executor->execs[0]);
// //         if (!path)
// //             return (NULL);
// //         if (access(path, F_OK) == -1 && printerror)
// //             get_path_error(shell->executor->execs[0]);
// //         return (path);
// //     }
// //     // otherwise searrch PATH
// //     path = get_absolute_path(shell);
// //     if (!path)
// //         return (NULL);
// //     if (access(path, F_OK) == -1 && printerror)
// //         // get_path_error(shell->executor->execs[0]);
    
// //     return (path);
// // }


// static int execute_other(t_executor *current, t_info *info)
// {
//     char    *path;
//     char    **env_array;
//     struct stat path_stat;

//     path = execute_other_helper(current);
//     if (!path)
//     {
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         ft_putstr_fd(current->execs[0], STDERR_FILENO);
//         ft_putstr_fd(": command not found\n", STDERR_FILENO);
//         exit(127); // Command not found
//     }

//     // Check if path is a directory
//     if (stat(path, &path_stat) == 0) {
//         if ((path_stat.st_mode & S_IFDIR) == S_IFDIR) {
//             ft_putstr_fd("minishell: ", STDERR_FILENO);
//             ft_putstr_fd(path, STDERR_FILENO);
//             ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
//             exit(126); // Path is a directory, not executable
//         }
//     }

//     env_array = transform_environ_array(info->shell);
//     if (!env_array)
//     {
//         exit(126); // Cannot create environment
//     }

//     execve(path, current->execs, env_array);
//     exit(126); // Execution failed
// }
// static int execute_other(t_executor *current, t_info *info)
// {
//     char    *path;
//     char    **env_array;

//     path = execute_other_helper(current);
//     // path = get_path(info->shell, true);
//     // printf("%s\n",path);
//     if (!path)
//     {
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         ft_putstr_fd(current->execs[0], STDERR_FILENO);
//         ft_putstr_fd(": command not found\n", STDERR_FILENO);
//         // free_heap(info->shell);
//         exit(127);
//     }

//     env_array = transform_environ_array(info->shell);
//     if (!env_array)
//     {
//         //free(path);
//         // free_heap(info->shell);
//         exit(126);
//     }

//     execve(path, current->execs, env_array);
//     // //free(path);
//     // free_str_arr(env_array);
//     // free_heap(info->shell);
//     // ft_putstr_fd("minishell: ", STDERR_FILENO);
//     // perror(current->execs[0]);
//     exit(126);
// }

// static void execute_builtin_child(t_info *info)
// {
//     int exit_code;
//     exit_code = handle_single_builtin(info->shell);
//     // free_heap(info->shell);
//     exit(exit_code);
// }

// old:
// static void execute_builtin_child(t_info *info)
// {
//     int exit_code;
//     exit_code = handle_single_builtin(info->shell);
//     exit(exit_code);
// }

// new:
static void execute_builtin_child(t_info *info, t_executor *cur)
{
    int exit_code = 0;
    char **args = cur->execs;

    if      (ft_strcmp(args[0], "echo")   == 0) exit_code = echo(args);
    else if (ft_strcmp(args[0], "cd")     == 0) exit_code = cd(info->shell, args);
    else if (ft_strcmp(args[0], "pwd")    == 0) exit_code = pwd(info->shell->env);
    else if (ft_strcmp(args[0], "export") == 0) exit_code = export(info->shell, args);
    else if (ft_strcmp(args[0], "unset")  == 0) exit_code = unset(info->shell, args);
    else if (ft_strcmp(args[0], "env")    == 0) exit_code = env(info->shell);
    else if (ft_strcmp(args[0], "exit")   == 0) exit_code = exit_builtin(info->shell, args, 1); //// if in shild not print exit

    exit(exit_code);
}


int child_handler_multi(int *fildes, t_executor *current, t_info *info)
{
    // if (handle_redirections_pipeline(fildes, current, info) == FAIL_SYSCALL)
    // {
    //     // free_heap(info->shell);
    //     exit(FAIL_SYSCALL_CHILD);
    // }
    if (handle_redirections_pipeline(fildes, current) == FAIL_SYSCALL)
        exit(FAIL_SYSCALL_CHILD);

    if (is_cmdline_empty(current->execs[0]))
    {
            // skip parsing
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(current->execs[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        exit(127);
    }
    if (is_builtin(current->execs[0]))
        execute_builtin_child(info, current);
    //else
    // if (current->fd_in != STDIN_FILENO)
    // {
    //     dup2(current->fd_in, STDIN_FILENO);
    //     close(current->fd_in);
    // }
    // if (current->fd_out != STDOUT_FILENO)
    // {
    //     dup2(current->fd_out, STDOUT_FILENO);
    //     close(current->fd_out);
    // }
    else
        execute_other(current, info);
    return (FAIL_SYSCALL_CHILD); // Should never reach here
}