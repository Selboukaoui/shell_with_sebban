/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:34:48 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/16 22:51:51 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Create main function of signals
// Create handling functions of ^\ and ^C

void    parent_handler(int signum)
{
    
}

void    setup_signals(int mode)
{
    struct sigaction sg;

    if (mode == 1) // parent
    {
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

