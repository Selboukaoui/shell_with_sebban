/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:13:33 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/29 12:43:12 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int 	pwd (t_environ_list *env)
{
    char *buf;
    t_environ_node  *node;

    
    buf = getcwd(NULL, 0);
    if (!buf)
    {
        node = ft_getenv(env, "PWD");
        if (!node || !node->value)
            return (ft_putstr_fd("PWD not set in environment\n", 2),1);
        printf ("%s\n",node->value);
    }
    else
    {
        printf ("%s\n", buf);
        free (buf);
    }
    return 0;
}
