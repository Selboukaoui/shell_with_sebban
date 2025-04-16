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
}