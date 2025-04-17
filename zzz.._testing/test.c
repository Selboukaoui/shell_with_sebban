
#include "../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void    parent_handler(int signum)
{
    write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void    setup_signals(int mode)
{
    struct sigaction sg;

    sigemptyset(&sg.sa_mask);
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


// int main()
// {
//     setup_signals(1);
//     while (true)
//     {
//         readline("samir: ");
//     }
// }