/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_newsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:20:59 by asebban           #+#    #+#             */
/*   Updated: 2025/04/17 16:04:41 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	int count = 0, i = 0;

	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break;

		if (s[i] == '\'' || s[i] == '"')
		{
			char quote = s[i++];
			while (s[i] && s[i] != quote)
				i++;
			if (s[i] == quote)
				i++;
			count++;
		}
		else
		{
			int op_len = ft_operator_len(&s[i]);
			if (op_len > 0)
			{
				count++;
				i += op_len;
			}
			else
			{
				while (s[i] && s[i] != ' ' && ft_operator_len(&s[i]) == 0 && s[i] != '\'' && s[i] != '"')
					i++;
				count++;
			}
		}
	}
	return count;
}

static char	*ft_dup_token(const char *str, int len)
{
	char *s = malloc(len + 1);
	if (!s)
		return (NULL);
	for (int i = 0; i < len; i++)
		s[i] = str[i];
	s[len] = '\0';
	return s;
}

static char	*extract_quoted(const char *s, int *i)
{
	char quote = s[(*i)++];
	int start = *i;

	while (s[*i] && s[*i] != quote)
		(*i)++;

	int len = *i - start;
	char *token = ft_dup_token(&s[start], len);

	if (s[*i] == quote)
		(*i)++; // skip closing quote

	return token;
}

char	**ft_newsplit(const char *s)
{
	if (!s)
		return NULL;

	int total = count_tokens(s);
	char **tokens = malloc(sizeof(char *) * (total + 1));
	if (!tokens)
		return NULL;

	int i = 0;
    int pos = 0;

	while (s[i])
	{
		while (s[i] == ' ')
			i++;
		if (!s[i])
			break;

		if (s[i] == '\'' || s[i] == '"')
		{
			tokens[pos++] = extract_quoted(s, &i);
		}
		else
		{
			int op_len = ft_operator_len(&s[i]);
			if (op_len > 0)
			{
				tokens[pos++] = ft_dup_token(&s[i], op_len);
				i += op_len;
			}
			else
			{
				int start = i;
				while (s[i] && s[i] != ' ' && ft_operator_len(&s[i]) == 0 && s[i] != '\'' && s[i] != '"')
					i++;
				tokens[pos++] = ft_dup_token(&s[start], i - start);
			}
		}
	}

	tokens[pos] = NULL;
	return tokens;
}

