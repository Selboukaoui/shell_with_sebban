/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:34:48 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/02 13:44:44 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "readline/readline.h"

// extern int  g_signal;

void    parent_handler(int signum)
{
    (void) signum;
    if (g_signals == 1)
    {
        write(STDOUT_FILENO, "^C\n", 3);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        exit_status(EXIT_SET,130);
    }
}

void    signal_setup(int mode)
{
    struct sigaction sg;

    sigemptyset(&sg.sa_mask);
    if (mode == 1)
    {
        sg.sa_handler = parent_handler;
        sg.sa_flags = SA_RESTART;
        sigaction(SIGINT, &sg, NULL);
        sg.sa_handler = SIG_IGN;
        sigaction(SIGQUIT, &sg, NULL);
        exit_status(EXIT_SET,130);
    }
    else
    {
        sg.sa_handler = SIG_DFL;
        sg.sa_flags = 0;
        sigaction(SIGINT, &sg, NULL);
        sg.sa_handler = SIG_IGN;
        sigaction(SIGQUIT, &sg, NULL);
        exit_status(EXIT_SET,130);
    }
    
}
