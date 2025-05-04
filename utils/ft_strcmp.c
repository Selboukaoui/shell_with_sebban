/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:06:26 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/04 20:45:48 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (((unsigned char)s1[i] || (unsigned char)s2[i]))
	{
		if ((unsigned char)s1[i] == (unsigned char) s2[i])
			i++;
		else
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	}
	return (0);
}


// char *ft_strndup(const char *s, size_t n)
// {
// 	char *dup;
// 	size_t len;

// 	if (!s)
// 		return (NULL);
// 	len = ft_strlen(s);
// 	if (n < len)
// 		len = n;
// 	dup = ft_malloc(len + 1, 1);
// 	if (!dup)
// 		return (NULL);
// 	ft_strlcpy(dup, s, len + 1);
// 	return (dup);
// }


