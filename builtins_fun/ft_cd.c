/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:03:20 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/20 17:08:39 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int cd_no_args(void)
{
    char    *home;

    if (!(home = getenv("HOME")))
        return(ft_putchar_fd("Home dir not set\n", 2), FAILED);
    if (!chdir(home))
        return(ft_putchar_fd("Chdir failed\n", 2), FAILED);
    return (OK);
}



int 	cd (t_shell *shell, char **arg)
{
    char    *old_pwd;
    char    *new_pwd;
    int     res;

    old_pwd = getcwd(NULL, 0);
    new_pwd = NULL;
    res = FAILED;
    if (!arg[1])
        if (cd_no_args() == FAILED)
            return (free (old_pwd), FAILED);
    else
        if (!chdir(arg[1]))
        return (free(old_pwd),ft_putchar_fd("Chdir failed\n", 2) ,FAILED);
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
        return (free(old_pwd),ft_putchar_fd("Path not found\n", 2) ,FAILED);
    
    

}