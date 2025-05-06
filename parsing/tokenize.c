/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:04 by asebban           #+#    #+#             */
/*   Updated: 2025/05/06 17:43:37 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	t_lexer_list	*create_lexer_list(char **input_array)
{
	t_lexer_list	*head;
	t_lexer_list	*current;
	t_lexer_list	*prev;
	int				size;
	int				i;

	if (!input_array)
		return (NULL);
	size = 0;
	while (input_array[size])
		size++;
	if (size == 0)
		return (NULL);
	head = (t_lexer_list *)ft_malloc(sizeof(t_lexer_list), 1);
	if (!head)
		return (NULL);
	head->prev = NULL;
	prev = head;
	i = 1;
	while (i < size)
	{
		current = (t_lexer_list *)ft_malloc(sizeof(t_lexer_list), 1);
		if (!current)
		{
			while (head)
			{
				current = head->next;
				head = current;
			}
			return (NULL);
		}
		current->prev = prev;
		prev->next = current;
		prev = current;
		i++;
	}
	if (size > 0)
		prev->next = NULL;
	return (head);
}

static	int	find_token_pos(const char *rl, const char *s, int last_pos)
{
	size_t	rem_len;
	char	*p;
	 
	rem_len = ft_strlen(rl + last_pos);
	p = ft_strnstr(rl + last_pos, s, rem_len);
	if (!p)
		return (-1);
	return (int)(p - rl);
}

static int is_inside_quotes(const char *rl, int idx)
{
	int  inquote    = 0;
	char quote_char = 0;

	for (int j = 0; j < idx; j++)
	{
		if (rl[j] == '\'' || rl[j] == '"')
		{
			if (!inquote)
			{
				inquote    = 1;
				quote_char = rl[j];
			}
			else if (quote_char == rl[j])
			{
				inquote    = 0;
				quote_char = 0;
			}
		}
	}
	return inquote;
}

t_token which_type(char *s, int inside)
{
	if (!s)
		return EMPTY;
	if (inside)
		return CMD;

	if (ft_strcmp(s, "|")  == 0) return PIPE;
	if (ft_strcmp(s, "<")  == 0) return REDERECT_IN;
	if (ft_strcmp(s, ">")  == 0) return rederect_out;
	if (ft_strcmp(s, "<<") == 0) return HEREDOC;
	if (ft_strcmp(s, ">>") == 0) return APPEND;
	return CMD;
}

t_lexer_list	*fill_lexer_list(char **input_array,
								const char *raw_rl,
								const char *rl_copy)
{
	t_lexer_list *head;
	t_lexer_list *cur;
	int           last_pos;
	int           idx;

	head = create_lexer_list(input_array);
	cur  = head;
	last_pos = 0;
	int override_all = 0;
	{
		int i = 0;
		while (raw_rl[i] == ' ' || raw_rl[i] == '\t')
			i++;
		if (raw_rl[i] == '$'
		 && (ft_isalpha(raw_rl[i+1]) || raw_rl[i+1] == '_'))
			override_all = 1;
	}

	for (int i = 0; input_array[i] && cur; i++, cur = cur->next)
	{
		cur->str = ft_strdup(input_array[i]);
		if (!cur->str)
		{
			return NULL;
		}

		idx = find_token_pos(rl_copy, cur->str, last_pos);
		if (idx < 0) idx = last_pos;
		last_pos = idx + ft_strlen(cur->str);

		if (override_all)
		{
			cur->type = CMD;
		}
		else
		{
			int inside = is_inside_quotes(rl_copy, idx);
			cur->type = which_type(cur->str, inside);
		}
	}

	return head;
}

t_lexer_list *tokenize(t_shell *shell)
{
	char **tokens;
	
	if	(!shell || !shell->rl_copy )
		return (NULL);
	tokens = ft_newsplit(shell->rl_copy);
	if (!tokens)
		return (NULL);
	t_lexer_list *lex = fill_lexer_list(tokens,
						shell->rl_input, shell->rl_copy);
	return (lex);
}
