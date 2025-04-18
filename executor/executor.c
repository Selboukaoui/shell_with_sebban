/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:42:43 by asebban           #+#    #+#             */
/*   Updated: 2025/04/19 11:35:09 by selbouka         ###   ########.fr       */
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
}