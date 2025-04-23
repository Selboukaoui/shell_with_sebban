/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:13:33 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/22 18:51:52 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int 	pwd (t_environ_list *env)
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
        printf ("%s\n", buf);
    return 0;
}

