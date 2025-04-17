/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:04 by asebban           #+#    #+#             */
/*   Updated: 2025/04/17 16:04:11 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token which_type(char *s)
{
    if (!s)
        return (EMPTY);
    
    if (ft_strcmp(s, "|") == 0)
        return (PIPE);
    else if (ft_strcmp(s, "<") == 0)
        return (REDERECT_IN);
    else if (ft_strcmp(s, ">") == 0)
        return (REDERECT_OUT);
    else if (ft_strcmp(s, "<<") == 0)
        return (HEREDOC);
    else if (ft_strcmp(s, ">>") == 0)
        return (APPEND);
    return (COMMAND);
}

static t_lexer_list *create_lexer_list(char **input_array)
{
    t_lexer_list    *head;
    t_lexer_list    *current;
    t_lexer_list    *prev;
    int             size;
    int             i;

    if (!input_array)
        return (NULL);
    
    // Calculate size of input array
    size = 0;
    while (input_array[size])
        size++;
    
    if (size == 0)
        return (NULL);
    
    // Allocate first node
    head = (t_lexer_list *)malloc(sizeof(t_lexer_list));
    if (!head)
        return (NULL);
    head->prev = NULL;
    prev = head;
    
    // Allocate remaining nodes
    i = 1;
    while (i < size)
    {
        current = (t_lexer_list *)malloc(sizeof(t_lexer_list));
        if (!current)
        {
            // Cleanup on allocation failure
            while (head)
            {
                current = head->next;
                free(head);
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

static t_lexer_list *fill_lexer_list(char **input_array)
{
    t_lexer_list    *head;
    t_lexer_list    *current;
    int             i;

    head = create_lexer_list(input_array);
    if (!head)
        return (NULL);
    
    current = head;
    i = 0;
    while (input_array[i] && current)
    {
        current->str = ft_strdup(input_array[i]);
        if (!current->str)
        {
            // Cleanup on allocation failure
            while (head)
            {
                current = head->next;
                free(head->str);
                free(head);
                head = current;
            }
            return (NULL);
        }
        current->type = which_type(current->str);
        current = current->next;
        i++;
    }
    
    return (head);
}

t_lexer_list *tokenize(t_shell *shell)
{
    char            **input_array;
    t_lexer_list    *lexer_list;

    if (!shell || !shell->rl_copy)
        return (NULL);
    input_array = ft_newsplit(shell->rl_copy);
    if (!input_array)
        return (NULL);
    
    lexer_list = fill_lexer_list(input_array);
    // free_str_arr(input_array);
    return (lexer_list);
}