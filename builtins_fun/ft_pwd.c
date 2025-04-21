/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:13:33 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/21 12:21:04 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int 	pwd (void)
{
    // char *buf;
    
    // if (!(buf = (char *)ft_malloc(4096)))
    //     return (perror("allocation failed \n"), 0);
    // if (getcwd(buf, 4096) == NULL)
    //     return (perror("getcwd failed \n"), 0);
    
    printf ("%s\ndyalii\n", getcwd(NULL, 0));
    // free (buf);
    return 0;
}

// int main (){pwd();}