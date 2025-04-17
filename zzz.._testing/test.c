
#include "../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void    parent_handler(int signum)
{
    rl_replace_line("", 0);

    printf ("***********\n");
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


int main()
{
    while (true)
    {
        setup_signals(1);
        pause();
    }
}