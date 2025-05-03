/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:39:36 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/03 14:04:21 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *save_pwd(int action, char *value)
{
    static char *save;

    if (action == 0)         // GET
        return save;
    else if (action == 1)    // SET
    {
        free(save); // free previous if any
        if (value)
            save = ft_strdup(value); // duplicate instead of storing pointer
        else
            save = NULL;
    }
    return save;
}
