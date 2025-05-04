/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:39:36 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/04 10:23:16 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*save_pwd(int action, char *value)
{
	static char	*save;

	if (action == 0)
		return (save);
	else if (action == 1)
	{
		free(save);
		if (value)
			save = ft_strdup(value);
		else
			save = NULL;
	}
	return (save);
}
