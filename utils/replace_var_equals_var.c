/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var_equals_var.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:51:53 by asebban           #+#    #+#             */
/*   Updated: 2025/05/07 19:50:49 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * var_chain_replacement.c
 *
 * Demonstration of replacing chained variable assignments in a command string.
 * Patterns like "$X=$Y=$Z" or "str=$str" get expanded using environment values.
 */
/*
 * Compliant Minishell variable-expansion functions without forbidden calls
 */



/* Append src to dest at position *o, update *o */
static void ft_append(char *dest, size_t *o, const char *src)
{
	for (size_t i = 0; src && src[i]; ++i)
		dest[(*o)++] = src[i];
	dest[*o] = '\0';
}

/* Expand a single segment: var=... chains */
char *expand_chain(const char *seg)
{
	size_t len = ft_strlen(seg);
	/* max expansion: each var replaced by env value; buffer = len * 256 */
	char *out = malloc(len * 256 + 1);
	if (!out) return NULL;
	size_t o = 0, i = 0;

	if (seg[i] == '$') {
		/* parse first variable name */
		++i;
		size_t start = i;
		while (seg[i] && (seg[i] == '_' || (seg[i] >= '0' && seg[i] <= '9') || (seg[i] >= 'A' && seg[i] <= 'Z') || (seg[i] >= 'a' && seg[i] <= 'z')))
			++i;
		size_t nlen = i - start;
		if (nlen > 0 && seg[i] == '=') {
			/* write value of first var */
			char name[256];
			for (size_t k = 0; k < nlen; ++k)
				name[k] = seg[start + k];
			name[nlen] = '\0';
			const char *val = getenv(name);
			if (val) ft_append(out, &o, val);
			out[o++] = '=';
			++i;
			/* chain subsequent $vars */
			while (seg[i] == '$') {
				++i;
				start = i;
				while (seg[i] && (seg[i] == '_' || (seg[i] >= '0' && seg[i] <= '9') || (seg[i] >= 'A' && seg[i] <= 'Z') || (seg[i] >= 'a' && seg[i] <= 'z')))
					++i;
				nlen = i - start;
				if (nlen == 0) break;
				for (size_t k = 0; k < nlen; ++k)
					name[k] = seg[start + k];
				name[nlen] = '\0';
				val = getenv(name);
				if (val) ft_append(out, &o, val);
				if (seg[i] == '=') {
					out[o++] = '=';
					++i;
				} else break;
			}
			/* copy rest */
			for (; seg[i]; ++i)
				out[o++] = seg[i];
			out[o] = '\0';
			return out;
		}
	}
	/* fallback: return duplicate */
	for (size_t k = 0; k <= len; ++k)
		out[k] = seg[k];
	return out;
}

char *replace_var_equals_var(const char *input)
{
	size_t  in_len = ft_strlen(input);
	char    *copy = ft_strdup(input);
	if (!copy)
		return NULL;
	char **segs = malloc((in_len / 2 + 1) * sizeof(char*));
	if (!segs) { free(copy); return NULL; }

	size_t count = 0;
	size_t i = 0;
	while (i < in_len) {
		/* skip spaces */
		while (i < in_len && copy[i] == ' ') ++i;
		if (i >= in_len) break;
		size_t start = i;
		while (i < in_len && copy[i] != ' ') ++i;
		size_t toklen = i - start;
		char *tok = malloc(toklen + 1);
		for (size_t k = 0; k < toklen; ++k)
			tok[k] = copy[start + k];
		tok[toklen] = '\0';
		segs[count++] = expand_chain(tok);
		free(tok);
	}
	free(copy);

	/* join segments */
	size_t total = 0;
	for (size_t j = 0; j < count; ++j)
		total += ft_strlen(segs[j]) + 1;
	char *result = malloc(total + 1);
	if (!result) return NULL;
	size_t o = 0;
	for (size_t j = 0; j < count; ++j) {
		ft_append(result, &o, segs[j]);
		if (j + 1 < count)
			result[o++] = ' ';
		free(segs[j]);
	}
	result[o] = '\0';
	free(segs);
	return result;
}
