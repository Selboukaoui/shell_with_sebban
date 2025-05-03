/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:13:33 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/03 13:52:58 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// if PWD not in env
int 	pwd (t_environ_list *env){
    char *buf;
    t_environ_node  *node;

    
    buf = getcwd(NULL, 0);
    if (!buf)
    {
        node = ft_getenv(env, "PWD");
        if (!node || !node->value)
            return (printf("--%s---\n" ,save_pwd(0, NULL)));

            // return (ft_putstr_fd("PWD not set in environment\n", 2),1); //pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
        printf ("%s\n",node->value);
    }
    else
    {
        printf ("%s\n", buf);
        free (buf);
    }
    return 0;
}



// int 	pwd (t_environ_list *env, int mode)
// {
//     char *buf;
//     t_environ_node  *node;
//     static char *save;
//     // if (mode)
//     // {
//         buf = getcwd(NULL, 0);
//         if (!buf)
//         {
//             node = ft_getenv(env, "PWD");
//             if (!node || !node->value)
//                 mode = 26;
//                 // return (ft_putstr_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory", 2),1); //pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
//             if (node && node->value && mode == 1)
//             {
//                 printf ("%s\n",node->value);
//             }
//             if (mode == 0)
//                 save = buf;
//         }
//         else
//         {
//             if (mode == 1)
//                 printf ("%s\n", buf);
//             save = buf;
//             // free (buf);
//         }
//     // }
//     if (mode == 26)
//     {
//         if (save)
//             printf ("%s\n", save);
//         else
//             ft_putstr_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
//     }
//         return 0;
// }
