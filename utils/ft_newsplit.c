/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_newsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:20:59 by asebban           #+#    #+#             */
/*   Updated: 2025/05/06 18:40:17 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strcat(char *dest, const char *src)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	while (dest[i])
		i++;
	while (src[j])
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest[i])
		i++;
	while (j < n && src[j])
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_dup_token(const char *str, int len)
{
	char	*s;
	int		i;

	if (!str || len <= 0)
		return (NULL);
	s = (char *)ft_malloc(len + 1, 1);
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

static	int	ft_operator_len(const char *s)
{
	if ((s[0] == '>' && s[1] == '>') || (s[0] == '<' && s[1] == '<'))
		return (2);
	else if (s[0] == '>' || s[0] == '<' || s[0] == '|')
		return (1);
	return (0);
}

static	int	count_tokens(const char *s)
{
	int		i;
	int		count;
	char	quote;
	int		op_len;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break ;
		op_len = ft_operator_len(&s[i]);
		if (op_len > 0)
		{
			count++;
			i += op_len;
			continue ;
		}
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != ' ' && ft_operator_len(&s[i]) == 0)
			{
				if (s[i] == '\'' || s[i] == '"')
				{
					quote = s[i++];
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
	return (count);
}

static	char	*append_segment(char *res, const char *seg, int len)
{
	char	*new_res;
	int		old_len;

	old_len = ft_strlen(res);
	new_res = ft_malloc(old_len + len + 1, 1);
	if (!new_res)
		return (NULL);
	ft_strcpy(new_res, res);
	ft_strncat(new_res, seg, len);
	return (new_res);
}

static int	parse_quoted(const char *s, int *i, char **res)
{
	char	quote;
	int		start;
	int		len;

	quote = s[(*i)++];
	start = *i;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	len = *i - start;
	*res = append_segment(*res, &s[start], len);
	if (!*res)
		return (0);
	if (s[*i] == quote)
		(*i)++;
	return (1);
}

static int	parse_literal(const char *s, int *i, char **res)
{
	int	start;
	int	len;

	start = *i;
	while (s[*i] && s[*i] != ' '
		&& ft_operator_len(&s[*i]) == 0
		&& s[*i] != '\'' && s[*i] != '"')
		(*i)++;
	len = *i - start;
	*res = append_segment(*res, &s[start], len);
	if (!*res)
		return (0);
	return (1);
}

char	*parse_token(const char *s, int *i)
{
	char	*res;

	res = ft_malloc(1, 1);
	if (!res)
		return (NULL);
	res[0] = '\0';
	while (s[*i] && s[*i] != ' ' && ft_operator_len(&s[*i]) == 0)
	{
		if (s[*i] == '\'' || s[*i] == '"')
		{
			if (!parse_quoted(s, i, &res))
				return (NULL);
		}
		else
		{
			if (!parse_literal(s, i, &res))
				return (NULL);
		}
	}
	return (res);
}

char	**ft_newsplit(const char *s)
{
	int		total;
	char	**tokens;
	int		i;
	int		pos;
	int		op_len;

	if (!s)
		return (NULL);
	total = count_tokens(s);
	tokens = ft_malloc(sizeof(char *) * (total + 1), 1);
	if (!tokens)
		return (NULL);
	i = 0;
	pos = 0;
	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break ;
		op_len = ft_operator_len(&s[i]);
		if (op_len > 0)
		{
			tokens[pos++] = ft_dup_token(&s[i], op_len);
			i += op_len;
		}
		else
			tokens[pos++] = parse_token(s, &i);
	}
	tokens[pos] = NULL;
	return (tokens);
}
