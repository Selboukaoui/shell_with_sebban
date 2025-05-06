/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:42:43 by asebban           #+#    #+#             */
/*   Updated: 2025/05/06 15:21:53 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static void    handle_single(t_shell *shell)
{
    t_executor *current = shell->executor;
    // printf ("current->ex[1] : %s\n",current->execs[0]);
    if (!current->execs || !current->execs[0])
    {
        exit_status(EXIT_SET, 0);// Empty command, no error
        return;
    }
    if (is_builtin(current->execs[0]))
    {
        if (handle_single_builtin(shell) == EXIT_FAILURE)
        {
            exit_status(EXIT_SET, 1); // Failed builtin (e.g., invalid cd)
        }
        else
        {
            exit_status(EXIT_SET, 0); // Success
        }
    }
    else
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("minishell");
            exit_status(EXIT_SET, 1);
            return ;
        }
        else if (pid == 0)
        {
            // signal(SIGQUIT, SIG_DFL)
            signal_setup(CHILD);
            handle_single_child(shell);
        }
        else
        {
            int status;
            waitpid (pid, &status, 0);
            
            if (WIFEXITED(status))
                exit_status(EXIT_SET, WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                exit_status(EXIT_SET, (128 + WTERMSIG(status)));
            else
                exit_status(EXIT_SET, 1);
            // check_exitcode(status, &(bool){true});
        }
    }
}

void executor(t_shell *shell)
{
    t_executor *current = shell->executor;
    if (!current)
        return;
    // printf ("size ---> %d\n", current->size);
    if (current->size == 1)
    {
        handle_single(shell);
    }
    else
    {
        t_info *info = init_info(shell);
        if (!info)
        {
            //not forgot here to return // check
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
