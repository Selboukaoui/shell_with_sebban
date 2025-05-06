/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 23:25:16 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/06 13:44:12 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	j;
	char	*p;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	while (i < len && s[start + i])
		i++;
	p = ft_malloc((i + 1) * sizeof(char), 1);
	if (!p)
		return (NULL);
	j = 0;
	while (len > 0 && s[start])
	{
		p[j++] = s[start++];
		len--;
	}
	p[j] = '\0';
	return ((char *)p);
}



// char	*ft_substr1(char const *s, unsigned int start, size_t len)
// {
// 	size_t	j;
// 	char	*p;
// 	size_t	i;

// 	i = 0;
// 	if (!s)
// 		return (NULL);
// 	if (start >= ft_strlen(s))
// 		return (ft_strdup1(""));
// 	while (i < len && s[start + i])
// 		i++;
// 	p = ft_malloc((i + 1) * sizeof(char));
// 	if (!p)
// 		return (NULL);
// 	j = 0;
// 	while (len > 0 && s[start])
// 	{
// 		p[j++] = s[start++];
// 		len--;
// 	}
// 	p[j] = '\0';
// 	return ((char *)p);
// }
