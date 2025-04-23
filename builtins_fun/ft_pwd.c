/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:13:33 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/23 12:25:32 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int 	pwd (t_environ_list *env, int t)
{
    char *buf;
    t_environ_node  *node;
    
    buf = ft_malloc(4096 , 0);
    getcwd(buf, 4096);
    if (!buf)
    {
        node = ft_getenv(env, "PWD");
        printf ("%s\n",node->value);
    }
    else
    {
        printf ("%s\n", buf);
        while (t > 0)
        {
            printf ("../");
            t--;
        }
    }
    return 0;
}

