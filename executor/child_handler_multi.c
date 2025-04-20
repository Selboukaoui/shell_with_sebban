/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_handler_multi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:38:38 by asebban           #+#    #+#             */
/*   Updated: 2025/04/20 14:33:50 by selbouka         ###   ########.fr       */
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


static char *path_join(const char *dir, const char *file)
{
    char *tmp;
    char *full;

    // dir + "/" 
    tmp = ft_strjoin(dir, "/");
    if (!tmp)
        return NULL;

    // (dir + "/") + file
    full = ft_strjoin(tmp, file);
    free(tmp);
    return full;
}


 static char *execute_other_helper(t_executor *current)
{
    char    *full_path;
    int     i;

    if (current->execs[0][0] == '/' || current->execs[0][0] == '.')
    {
        if (access(current->execs[0], F_OK | X_OK) == 0)
            return (ft_strdup(current->execs[0]));
        return (NULL);
    }

    i = 0;
    // while (current->path[i])
    // {
    //     printf("-----------__>%s\n", current->path[i++]);
    // }
    // i = 0;
    // while (current->path && current->path[i])
    // {
    //     full_path = ft_strjoin(current->path[i], current->execs[0]);
    //     if (!full_path)
    //         return (NULL);
    //     if (access(full_path, F_OK | X_OK) == 0)
    //         return (full_path);
    //     free(full_path);
    //     i++;
    // }
    i = 0;
        while (current->path && current->path[i])
        {
            full_path = path_join(current->path[i], current->execs[0]);
            if (!full_path)
                return NULL;
            if (access(full_path, F_OK | X_OK) == 0)
                return full_path;
            free(full_path);
            i++;
        }
    // printf("here\n");
    return (NULL);
}

// char *get_path(t_shell *shell, bool printerror)
// {
//     char    *path;
//     if(!shell->executor->execs || !shell->executor->execs[0])
//         return(NULL);
//     // If command is already an absolute path
//     if (ft_strchr((shell->executor->execs[0]), '/'))
//     {
//         path = ft_strdup(shell->executor->execs[0]);
//         if (!path)
//             return (NULL);
//         if (access(path, F_OK) == -1 && printerror)
//             get_path_error(shell->executor->execs[0]);
//         return (path);
//     }
//     // otherwise searrch PATH
//     path = get_absolute_path(shell);
//     if (!path)
//         return (NULL);
//     if (access(path, F_OK) == -1 && printerror)
//         // get_path_error(shell->executor->execs[0]);
    
//     return (path);
// }

static int execute_other(t_executor *current, t_info *info)
{
    char    *path;
    char    **env_array;

    path = execute_other_helper(current);
    // path = get_path(info->shell, true);
    // printf("%s\n",path);
    // if (!path)
    // {
    //     ft_putstr_fd("minishell: ", STDERR_FILENO);
    //     ft_putstr_fd(current->execs[0], STDERR_FILENO);
    //     ft_putstr_fd(": command not found\n", STDERR_FILENO);
    //     free_heap(info->shell);
    //     exit(127);
    // }

    env_array = transform_environ_array(info->shell);
    // if (!env_array)
    // {
    //     free(path);
    //     free_heap(info->shell);
    //     exit(126);
    // }

    execve(path, current->execs, env_array);
    // free(path);
    // free_str_arr(env_array);
    // free_heap(info->shell);
    // ft_putstr_fd("minishell: ", STDERR_FILENO);
    // perror(current->execs[0]);
    exit(126);
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

    // if (is_builtin(current->execs[0]))
    //     execute_builtin_child(current, info)
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
    execute_other(current, info);
    return (FAIL_SYSCALL_CHILD); // Should never reach here
}