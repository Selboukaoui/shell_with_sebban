/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:42:43 by asebban           #+#    #+#             */
/*   Updated: 2025/04/20 13:23:31 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static void    handle_single(t_shell *shell)
{
    t_executor *current = shell->executor;
    // printf ("current->ex[1] : %s\n",current->execs[0]);
    if (!current->execs || !current->execs[0])
    {
        //exit code
        return;
    }
    // if (is_builtin(current->execs[0]))
    // {
        
    // }
    else
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("minishell");
            //exit_code
            return;
        }
        else if (pid == 0)
        {
            handle_single_child(shell);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            // check_exitcode(status, &(bool){true});
        }
    }
}

void executor(t_shell *shell)
{
    t_executor *current = shell->executor;
    if (!current)
        return;
    printf ("size ---> %d\n", current->size);
    if (current->size == 1)
    {
        handle_single(shell);
    }
    else
    {
        t_info *info = init_info(shell);
        if (!info)
        {
            //not forgot here to return
        }
        while (current)
        {
            handle_multi(info, current);
            current = current->next;
        }

        wait_pipeline(info, shell->executor->size);
        // free(info->pids);
        // free(info);
    }
}