/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:21:01 by asebban           #+#    #+#             */
/*   Updated: 2025/05/08 22:21:14 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin2(char *s1, char *s2, int to_free)
{
    size_t  len1 = s1 ? ft_strlen(s1) : 0;
    size_t  len2 = s2 ? ft_strlen(s2) : 0;
    char   *res = malloc(len1 + len2 + 1);

    if (!res)
        return (NULL);

    if (s1)
        ft_strcpy(res, s1);
    else
        res[0] = '\0';

    if (s2)
        ft_strcat(res, s2);

    res[len1 + len2] = '\0';

    if (to_free & 1)
        free(s1);
    if (to_free & 2)
        free(s2);

    return (res);
}