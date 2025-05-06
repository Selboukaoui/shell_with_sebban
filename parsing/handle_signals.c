/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:34:48 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/06 17:19:32 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "readline/readline.h"
#include <asm-generic/signal-defs.h>

extern int  g_signals;

void    parent_handler(int signum)
{
    (void)signum;
    if (g_signals == 0)
    {
        write(STDOUT_FILENO, "^C\n", 4);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        exit_status(EXIT_SET,130);
    }

}

// void    heredoc_handler(int signum)
// {
//     (void) signum;
//     // if (g_signals == 0)
//     // {
//         write(STDOUT_FILENO, "^C\n", 4);
//         rl_on_new_line();
//         rl_replace_line("", 0);
//         rl_redisplay();
//         exit_status(EXIT_SET,130);
//     // }

// }

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
    // else if (mode == 2)
    // {
    //     sg.sa_handler = heredoc_handler;
    //     sg.sa_flags = SA_RESTART;
    //     sigaction(SIGINT, &sg, NULL);
    // }
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

// void    signal_setup_heredoc_main(void)
// {
//     signal(SIGINT, sigint_handler_heredoc);
//     signal(SIGQUIT, SIG_IGN);
// }

// void    signal_restore_main(void)
// {
//     signal(SIGINT, your_main_sigint_handler);  // restore your shell's SIGINT
//     signal(SIGQUIT, your_main_sigquit_handler);
// }
// static volatile sig_atomic_t    g_heredoc_interrupted = 0;

// void    sigint_handler_heredoc(int sig)
// {
//     (void)sig;
//     write(STDOUT_FILENO, "\n", 1);
//     g_heredoc_interrupted = 1;
//     rl_replace_line("", 0);
//     rl_on_new_line();
//     rl_redisplay();
// }