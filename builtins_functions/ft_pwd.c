/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:13:33 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/20 14:26:23 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int 	pwd (void)
{
    long size = pathconf(".", _PC_PATH_MAX);
    char *buf = (char *)malloc((size_t)size);
    printf ("%s\n", getcwd(buf, size));
    return 0;
}

int main()
{
    pwd();
}
