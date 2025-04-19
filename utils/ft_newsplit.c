/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_newsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:20:59 by asebban           #+#    #+#             */
/*   Updated: 2025/04/19 14:40:34 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_strcat(char *dest, const char *src)
{
	size_t i = 0;
	size_t j = 0;

	while (dest[i])
		i++;

	while (src[j])
	{
		dest[i + j] = src[j];
		j++;
	}

	dest[i + j] = '\0';
	return dest;
}


char *ft_strncat(char *dest, const char *src, size_t n)
{
	size_t i = 0;
	size_t j = 0;

	// Find the end of dest
	while (dest[i])
		i++;

	// Append at most n characters from src
	while (j < n && src[j])
	{
		dest[i + j] = src[j];
		j++;
	}

	// Null-terminate the result
	dest[i + j] = '\0';

	return dest;
}

char *ft_dup_token(const char *str, int len)
{
    char *s;
    int i;

    if (!str || len <= 0)
        return (NULL);

    s = (char *)malloc(len + 1);
    if (!s)
        return (NULL);

    i = 0;
    while (i < len && str[i])
    {
        s[i] = str[i];
        i++;
    }
    s[i] = '\0';
    return (s);
}

static int	ft_operator_len(const char *s)
{
	if ((s[0] == '>' && s[1] == '>') || (s[0] == '<' && s[1] == '<'))
		return (2);
	else if (s[0] == '>' || s[0] == '<' || s[0] == '|')
		return (1);
	return (0);
}

static int	count_tokens(const char *s)
{
	int i = 0, count = 0;

	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break;

		int op_len = ft_operator_len(&s[i]);
		if (op_len > 0)
		{
			count++;
			i += op_len;
			continue;
		}

		if (s[i])
		{
			count++;
			while (s[i] && s[i] != ' ' && ft_operator_len(&s[i]) == 0)
			{
				if (s[i] == '\'' || s[i] == '"')
				{
					char quote = s[i++];
					while (s[i] && s[i] != quote)
						i++;
					if (s[i] == quote)
						i++;
				}
				else
					i++;
			}
		}
	}
	return count;
}

static char	*parse_token(const char *s, int *i)
{
	// int start = *i;
	char *res = malloc(1);
	if (!res)
		return NULL;
	res[0] = '\0';

	while (s[*i] && s[*i] != ' ' && ft_operator_len(&s[*i]) == 0)
	{
		if (s[*i] == '\'' || s[*i] == '"')
		{
			char quote = s[(*i)++];
			int sub_start = *i;
			while (s[*i] && s[*i] != quote)
				(*i)++;
			int len = *i - sub_start;

			char *quoted = ft_dup_token(&s[sub_start], len);
			char *tmp = res;
			res = malloc(ft_strlen(tmp) + len + 1);
			if (!res)
				return NULL;
			ft_strcpy(res, tmp);
			ft_strncat(res, quoted, len);
			free(tmp);
			free(quoted);

			if (s[*i] == quote)
				(*i)++;
		}
		else
		{
			int start_sub = *i;
			while (s[*i] && s[*i] != ' ' && ft_operator_len(&s[*i]) == 0 && s[*i] != '\'' && s[*i] != '"')
				(*i)++;
			int len = *i - start_sub;
			char *literal = ft_dup_token(&s[start_sub], len);
			char *tmp = res;
			res = malloc(ft_strlen(tmp) + len + 1);
			if (!res)
				return NULL;
			ft_strcpy(res, tmp);
			ft_strncat(res, literal, len);
			free(tmp);
			free(literal);
		}
	}
	return res;
}

char	**ft_newsplit(const char *s)
{
	if (!s)
		return NULL;

	int total = count_tokens(s);
	char **tokens = malloc(sizeof(char *) * (total + 1));
	if (!tokens)
		return NULL;

	int i = 0, pos = 0;

	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break;

		int op_len = ft_operator_len(&s[i]);
		if (op_len > 0)
		{
			tokens[pos++] = ft_dup_token(&s[i], op_len);
			i += op_len;
		}
		else
			tokens[pos++] = parse_token(s, &i);
	}
	tokens[pos] = NULL;
	return tokens;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// static int	ft_operator_len(const char *s)
// {
// 	if ((s[0] == '>' && s[1] == '>') || (s[0] == '<' && s[1] == '<'))
// 		return (2);
// 	else if (s[0] == '>' || s[0] == '<' || s[0] == '|')
// 		return (1);
// 	return (0);
// }

// static char *ft_dup_token(const char *str, int len)
// {
//     char *s = (char *)malloc(len + 1);
//     if (!s)
//         return NULL;

//     for (int i = 0; i < len; i++)
//         s[i] = str[i];
//     s[len] = '\0';

//     return s;
// }

// static char *parse_token(const char *s, int *i)
// {
//     int start = *i;
//     char *res = malloc(1);
//     if (!res)
//         return NULL;
//     res[0] = '\0';

//     while (s[*i] && s[*i] != ' ' && ft_operator_len(&s[*i]) == 0)
//     {
//         if (s[*i] == '\'' || s[*i] == '"')
//         {
//             char quote = s[(*i)++];
//             int sub_start = *i;
//             while (s[*i] && s[*i] != quote)
//                 (*i)++;
//             int len = *i - sub_start;

//             char *quoted = ft_dup_token(&s[sub_start], len);
//             if (!quoted) return NULL;

//             char *tmp = res;
//             res = malloc(strlen(tmp) + len + 1);
//             if (!res) return NULL;
//             strcpy(res, tmp);
//             strcat(res, quoted);
//             free(tmp);
//             free(quoted);

//             if (s[*i] == quote)
//                 (*i)++;
//         }
//         else
//         {
//             int start_sub = *i;
//             while (s[*i] && s[*i] != ' ' && ft_operator_len(&s[*i]) == 0 && s[*i] != '\'' && s[*i] != '"')
//                 (*i)++;
//             int len = *i - start_sub;
//             char *literal = ft_dup_token(&s[start_sub], len);
//             if (!literal) return NULL;

//             char *tmp = res;
//             res = malloc(strlen(tmp) + len + 1);
//             if (!res) return NULL;
//             strcpy(res, tmp);
//             strcat(res, literal);
//             free(tmp);
//             free(literal);
//         }
//     }
//     return res;
// }

// static int count_tokens(const char *s)
// {
//     int i = 0, count = 0;

//     while (s[i])
//     {
//         while (s[i] == ' ') // Skip over spaces
//             i++;

//         if (!s[i])
//             break;

//         // Check if we have an operator (>, <, |, >>, <<)
//         int op_len = ft_operator_len(&s[i]);
//         if (op_len > 0)
//         {
//             count++; // Found an operator, count it
//             i += op_len; // Move past the operator
//             continue;
//         }

//         // Handle normal token (non-operator and non-space)
//         if (s[i])
//         {
//             count++; // Found a token, count it
//             while (s[i] && s[i] != ' ' && ft_operator_len(&s[i]) == 0) // Skip over non-space, non-operator chars
//             {
//                 // If we encounter quotes, skip the quoted string
//                 if (s[i] == '\'' || s[i] == '"')
//                 {
//                     char quote = s[i++];
//                     while (s[i] && s[i] != quote)
//                         i++; // Skip over content inside quotes
//                     if (s[i] == quote)
//                         i++; // Skip the closing quote
//                 }
//                 else
//                     i++; // Otherwise just move one character forward
//             }
//         }
//     }

//     return count;
// }


// char **ft_newsplit(const char *s)
// {
//     if (!s)
//         return NULL;

//     int total = count_tokens(s); // Assuming count_tokens is correct
//     char **tokens = malloc(sizeof(char *) * (total + 1));
//     if (!tokens)
//         return NULL;

//     int i = 0, pos = 0;

//     while (s[i])
//     {
//         while (s[i] == ' ')
//             i++;
//         if (!s[i])
//             break;

//         int op_len = ft_operator_len(&s[i]);
//         if (op_len > 0)
//         {
//             tokens[pos++] = ft_dup_token(&s[i], op_len);
//             i += op_len;
//         }
//         else
//             tokens[pos++] = parse_token(s, &i);
//     }
//     tokens[pos] = NULL;
//     return tokens;
// }
