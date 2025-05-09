/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:33:32 by asebban           #+#    #+#             */
/*   Updated: 2025/05/09 14:11:00 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || \
			c == '\v' || c == '\f' || c == '\r');
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

void	int_to_str(int num, char *str)
{
	int		i;
	int		j;
	int		is_negative;
	char	tmp;

	i = 0;
	j = 0;
	is_negative = 0;
	if (num == 0)
	{
		str[i++] = '0';
		str[i] = '\0';
		return ;
	}
	if (num < 0)
	{
		is_negative = 1;
		num = -num;
	}
	while (num)
	{
		str[i++] = (num % 10) + '0';
		num /= 10;
	}
	if (is_negative)
		str[i++] = '-';
	str[i] = '\0';
	while (j < i / 2)
	{
		tmp = str[j];
		str[j] = str[i - j - 1];
		str[i - j - 1] = tmp;
		j++;
	}
}

char	*get_env_value(t_environ_list *env_list, char *key)
{
	t_environ_node	*cur;

	cur = env_list->head;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

static	int	already_quoted(const char *s)
{
	size_t n = strlen(s);
	if (n < 2) return 0;
	if (s[0] == '\'' && s[n-1] == '\'') return 1;
	if (s[0] == '"'  && s[n-1] == '"')  return 1;
	return 0;
}


static char *auto_quote_word(const char *word)
{
	int need = 0;
	for (const char *p = word; *p; p++)
		if (*p == ' ' || *p == '\t' || *p == '\'')
			{ need = 1; break; }

	if (!need)
		return ft_strdup(word);

	size_t len = ft_strlen(word);
	char *out = ft_malloc(len + 3, 1);
	if (!out) return NULL;
	sprintf(out, "\"%s\"", word);
	return out;
}
		
char *replace_vars(char *input, t_shell *shell)
{
	size_t i = 0, j = 0, L = ft_strlen(input);
	char  *output = ft_malloc(PATH_MAX + 1, 1);
	int    in_sq = 0, in_dq = 0;
	int    seen_export = 0, in_export_assign = 0;
	int    in_hdoc = 0, hdoc_word_started = 0;

	{
		size_t p = 0;
		while (p < L && ft_isspace(input[p])) p++;
		if (p + 6 <= L && ft_strncmp(&input[p], "export", 6) == 0
			&& (p + 6 == L || ft_isspace(input[p+6])))
			seen_export = 1;
	}

	while (i < L)
	{
		if (!in_sq && !in_dq && input[i] == '<' && input[i+1] == '<')
		{
			in_hdoc = 1;
			hdoc_word_started = 0;
			output[j++] = input[i++];
			output[j++] = input[i++];
			continue;
		}
		if (in_hdoc)
		{
			char c = input[i];
			if (!hdoc_word_started)
			{
				if (ft_isspace(c))
				{
					output[j++] = c;
					i++;
					continue;
				}
				hdoc_word_started = 1;
			}
			if (hdoc_word_started)
			{
				if (ft_isspace(c) || c == '|')
				{
					output[j++] = c;
					i++;
					in_hdoc = 0;
					continue;
				}
				output[j++] = c;
				i++;
				continue;
			}
		}
		if (seen_export && !in_export_assign && !in_sq && !in_dq && input[i] == '=')
			{ in_export_assign = 1; output[j++] = input[i++]; continue; }
		if (in_export_assign && !in_sq && !in_dq
			&& (ft_isspace(input[i]) || input[i] == '|'))
			{ in_export_assign = 0; output[j++] = input[i++]; continue; }
		if (input[i] == '\'' && !in_dq)
			{ in_sq = !in_sq; output[j++] = input[i++]; continue; }
		if (input[i] == '"' && !in_sq)
			{ in_dq = !in_dq; output[j++] = input[i++]; continue; }
		if (input[i] == '$' && !in_sq)
		{
			if (input[i+1] == '\0')        { output[j++] = '$'; i++; continue; }
			if (input[i+1] == '?')
			{
				int st; char buf[16];
				st = exit_status(0,0);
				int_to_str(st, buf);
				for (int k = 0; buf[k]; k++) output[j++] = buf[k];
				i += 2;
				continue;
			}
			if (ft_isdigit(input[i+1]))    { i += 2; continue; }
			if (ft_isalpha(input[i+1]) || input[i+1] == '_')
			{
				size_t vs = i+1, vl = 0;
				while (ft_isalnum(input[vs+vl]) || input[vs+vl] == '_') vl++;
				char name[vl+1];
				ft_strncpy(name, input + vs, vl);
				name[vl] = '\0';
				char *val = get_env_value(shell->env, name);
				if (val)
				{
					if (seen_export && in_export_assign)
					{
						if (!already_quoted(val)) output[j++] = '"';
						for (int k=0; val[k]; k++) output[j++] = val[k];
						if (!already_quoted(val)) output[j++] = '"';
					}
					else if (!in_dq)
					{
						char **words = ft_split(val, ' ');
						for (int w = 0; words[w]; w++)
						{
							char *q = auto_quote_word(words[w]);
							for (int k=0; q[k]; k++) output[j++] = q[k];
							if (words[w+1]) output[j++] = ' ';
						}
					}
					else
					{
						for (int k = 0; val[k]; k++) output[j++] = val[k];
					}
				}
				i = vs + vl;
				continue;
			}
		}
		output[j++] = input[i++];
	}
	output[j] = '\0';
	return output;
}
