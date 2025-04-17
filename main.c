/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:02:28 by asebban           #+#    #+#             */
/*   Updated: 2025/04/16 14:02:28 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"


int main(int ac, char **av, char **env)
{
    t_shell *shell;

    (void)ac;
    (void)av;
    signal_setup(PARENT);

    shell = init_shell(env);
    if (!shell)
        return (1);
    while (1)
    {
        shell->rl_input = readline("minishell-1.0$~ ");
        if (!shell->rl_input) // CTRL+D (EOF)
		{
			write(1, "exit\n", 5);
			break ;
		}
        if (*shell->rl_input)
            add_history(shell->rl_input);
        shell->rl_copy = clean_rl_copy(shell);
        if (!check_quote_syntax(shell->rl_copy))
        {
            // ft_putstr error for invalid syntax
            // exit code is : '258' I geuss
        }
        //check syntax like ">>>"
        if (!parser(shell))
        {
            // if input is null or mybe error 
            // parrsing input
        }
        // executor(shell);
        // and clean for next loop
    }
    // clean 
    return(0);// return exit_code
}