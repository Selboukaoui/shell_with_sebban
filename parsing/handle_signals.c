/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:34:48 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/21 15:38:20 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "readline/readline.h"

void    parent_handler(int signum)
{
    (void) signum;
    write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void    signal_setup(int mode)
{
    struct sigaction sg;

    sigemptyset(&sg.sa_mask);
    if (mode == 1) // parent
    {
        printf ("ana hna \n");
        sg.sa_handler = parent_handler;
        sg.sa_flags = SA_RESTART;
        sigaction(SIGINT, &sg, NULL);
        sg.sa_handler = SIG_IGN;
        sigaction(SIGQUIT, &sg, NULL);
    }
    else
    {
        sg.sa_handler = SIG_DFL;
        sg.sa_flags = 0;
        sigaction(SIGINT, &sg, NULL);
        sg.sa_handler = SIG_IGN;
        sigaction(SIGQUIT, &sg, NULL);
    }
}
