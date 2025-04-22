/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:14:24 by asebban           #+#    #+#             */
/*   Updated: 2025/04/22 14:17:22 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int env(t_shell *shell)
{
    t_environ_node *current;
    
    current = shell->env->head;
    while (current)
    {
        if (current->value)
        {
            // exit(200);
            ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
        }
        current = current->next;
    }
	return (EXIT_SUCCESS);
}

// int env(t_shell *shell)
// {
//     t_environ_node *current = shell->env->head;

//     while (current)
//     {
//         // ✅ Only print if the variable has a value
//         if (current->value)
//         {
//             ft_putstr_fd(current->key, STDOUT_FILENO);
//             ft_putchar_fd('=', STDOUT_FILENO);
//             ft_putstr_fd(current->value, STDOUT_FILENO);
//             ft_putchar_fd('\n', STDOUT_FILENO);
//         }
//         current = current->next;
//     }
//     return (EXIT_SUCCESS);
// }
