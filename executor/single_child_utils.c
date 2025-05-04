/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_child_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 10:02:46 by asebban           #+#    #+#             */
/*   Updated: 2025/05/04 20:36:28 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool handle_redirections_single_child(t_shell *shell)
{
    if (shell->executor->fd_in != STDIN_FILENO)
    {
        if (dup2(shell->executor->fd_in, STDIN_FILENO) == -1)
        {
            //error
        }
        close(shell->executor->fd_in);
    }
    if (shell->executor->fd_out != STDOUT_FILENO)
    {
        if (dup2(shell->executor->fd_out, STDOUT_FILENO) == -1)
        {
            //error
        }
        close(shell->executor->fd_out);
    }
    return (true);
}

static char *get_env1_value(t_shell *shell, const char *key)
{
    t_environ_node *env;
    size_t key_len;

    if (!shell || !shell->env || !key)
        return (NULL);

    key_len = ft_strlen(key);
    env = shell->env->head;
    while (env)
    {
        if (ft_strncmp(env->key, key, key_len) == 0 && env->key[key_len] == '\0')
            return (env->value); // Return pointer to value directly (don't duplicate unless you plan to modify it)
        env = env->next;
    }
    return (NULL);
}

// static char *get_absolute_path(t_shell *shell)
// {
//     char    *path_var;
//     char    **path_dirs;
//     char    *full_path;
//     int     i;
//     if (!shell->executor->execs || !shell->executor->execs[0])
//         return (NULL);
//     path_var = get_env_value(shell, "PATH");
//     if (!path_var)
//         return (ft_strdup(shell->executor->execs[0]));
//     path_dirs = ft_split(path_var, ':');
//     if (!path_dirs)
//         return (ft_strdup(shell->executor->execs[0]));
//     i = 0;
//     while (path_dirs[i])
//     {
//         // full_path = ft_strjoin3(path_dirs[i], "/", shell->executor->execs[0]);
//         char *temp;
//         temp = ft_strjoin(path_dirs[i], "/");
//         if (!temp)
//             return (free_str_arr(path_dirs),NULL);
//         full_path = ft_strjoin(temp, shell->executor->execs[0]);
//         // //free(temp);
//         if (!full_path)
//             return (free_str_arr(path_dirs),NULL);
//         if (access(full_path, F_OK) == 0)
//         {
//             free_str_arr(path_dirs);
//             return (full_path);
//         }
//         // //free(full_path);
//         i++;
//     }
//     free_str_arr(path_dirs);
//     return (ft_strdup(shell->executor->execs[0]));
// }

static char *get_absolute_path(t_shell *shell)
{
    char  *path_var;
    char **path_dirs;
    char  *full_path;
    char  *tmp;
    int    i;

    if (!shell->executor->execs[0])
        return NULL;

    path_var = get_env1_value(shell, "PATH");
    if (!path_var)
        return NULL;

    path_dirs = ft_split(path_var, ':');
    if (!path_dirs)
        return NULL;

    i = 0;
    while (path_dirs[i])
    {
        /* skip empty or "." entries */
        if (path_dirs[i][0] == '\0'
         || (path_dirs[i][0] == '.' && path_dirs[i][1] == '\0'))
        {
            i++;
            continue;
        }

        /* build "<dir>/<cmd>" */
        tmp = ft_strjoin(path_dirs[i], "/");
        if (!tmp)
            break;
        full_path = ft_strjoin(tmp, shell->executor->execs[0]);
        // free(tmp);
        if (!full_path)
            break;

        /* checnk existence and executability */
        if (access(full_path, X_OK) == 0)
        {
            // free_str_arr(path_dirs);
            return full_path;
        }

        // free(full_path);
        i++;
    }

    // free_str_arr(path_dirs);
    return NULL;
}

static void get_path_error(char *str)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(str, STDERR_FILENO);
    ft_putstr_fd(": command not found\n", STDERR_FILENO);
    // errno = 127;
}


char *get_path(t_shell *shell, bool printerror)
{
    char    *path;
    if(!shell->executor->execs || !shell->executor->execs[0])
        return(NULL);
    // If command is already an absolute path
    if (ft_strchr((shell->executor->execs[0]), '/'))
    {
        path = ft_strdup(shell->executor->execs[0]);
        if (!path)
            return (NULL);
        if (access(path, F_OK) == -1 && printerror)
            return(get_path_error(shell->executor->execs[0]), NULL);
        return (path);
    }
    // otherwise searrch PATH
    path = get_absolute_path(shell);
    if (!path)
        return (NULL);
    if (access(path, F_OK) == -1 && printerror)
        return(get_path_error(shell->executor->execs[0]), NULL);
    
    return (path);
}