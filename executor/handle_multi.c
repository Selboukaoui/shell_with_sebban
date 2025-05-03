/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_multi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:31:19 by asebban           #+#    #+#             */
/*   Updated: 2025/05/03 21:11:08 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_info *init_info(t_shell *shell)
{
    t_info *info;

    info = (t_info *)ft_calloc(1, sizeof(t_info));
    if (!info)
        return (NULL);
    
    info->shell = shell;
    info->stdin_copy = dup(STDIN_FILENO);
    info->pids = (pid_t *)ft_calloc(shell->executor->size, sizeof(pid_t));
    if (!info->pids)
    {
        // free(info);
        return (NULL);
    }
    return (info);
}

void wait_pipeline(t_info *info, int number)
{
    int status;
    int i;

    i = 0;
    while (i < number)
    {
        waitpid(info->pids[i], &status, 0);
        // if (WIFEXITED(status) && WEXITSTATUS(status) != 79)
        //     g_exit_code = WEXITSTATUS(status);
        // else if (WIFSIGNALED(status))
        //     g_exit_code = 128 + WTERMSIG(status);
        if (WIFEXITED(status)) 
        {
            exit_status(EXIT_SET, WEXITSTATUS(status)); // Handle normal exit
        }
        else if (WIFSIGNALED(status))
        {
            exit_status(EXIT_SET, 128 + WTERMSIG(status)); // Handle signal exit
        }        
        i++;
    }
    if (info->stdin_copy != STDIN_FILENO)
    {
        dup2(info->stdin_copy, STDIN_FILENO);
        close(info->stdin_copy);
    }
    // free(info->pids);
    // free(info);
}
void handle_multi(t_info *info, t_executor *current)
{
    int     fildes[2];
    int     ret;
    if (current->next)
    {
        if (pipe(fildes) == -1)
        {
            perror("minishell: pipe");
            // free(info->pids);
            // free(info);
            //exit_code
            return;
        }
        // current->fd_out = fildes[1];
        // current->next->fd_in = fildes[0];
        if (current->fd_out == STDOUT_FILENO) // respect redirection
            current->fd_out = fildes[1];
        else
            close(fildes[1]); // we won't use this pipe write-end

        if (current->next->fd_in == STDIN_FILENO) // respect heredoc or <
            current->next->fd_in = fildes[0];
        else
            close(fildes[0]); // we won't use this pipe read-end
    }
    info->pids[current->id] = fork();
    if (info->pids[current->id] == -1)
    {
        perror("minishell: fork");
        // free(info->pids);
        // free(info);
        // exit_code = 1;
        return;
    }
    else if (info->pids[current->id] == 0)
    {
        ret = child_handler_multi(fildes, current, info);
        exit(ret);
    }

    if (current->fd_in != STDIN_FILENO)
        close(current->fd_in);
    if (current->fd_out != STDOUT_FILENO)
        close(current->fd_out);
}
 