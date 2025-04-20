// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   prep_executor.c                                    :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/04/17 16:08:09 by asebban           #+#    #+#             */
// /*   Updated: 2025/04/17 16:50:49 by asebban          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../includes/minishell.h"

//typedef struct s_executor
// {
//     int                 id;
//     int                 size;
//     int                 fd_in;
//     int                 fd_out;
//     bool                truncate;
//     bool                append;
//     bool                redirect_input;
//     bool                heredoc;
//     char                **path;
//     char                **execs;
//     struct s_executor   *prev;
//     struct s_executor   *next;
// }               t_executor;

static int get_size_executor(t_lexer_list *head)
{
    t_lexer_list *current = head;
    int size = 1;

    while (current)
    {
        if (current->type == PIPE)
            size++;
        current = current->next;
    }
    return (size);
}

static t_executor *create_executor_list(int size)
{
    t_executor *head = NULL;
    t_executor *current = NULL;
    t_executor *new_node = NULL;

    while (size--)
    {
        new_node = (t_executor *)ft_calloc(1, sizeof(t_executor));
        if (!new_node)
            return (free_executor_list(head));
        
        if (!head)
            head = new_node;
        else
            current->next = new_node;
        
        new_node->prev = current;
        current = new_node;
    }
    return (head);
}

static void set_executor_defaults(t_executor *node)
{
    if (!node)
        return;
    
    node->fd_in = STDIN_FILENO;
    node->fd_out = STDOUT_FILENO;
    node->REDERECT_OUT = false;
    node->append = false;
    node->redirect_input = false;
    node->heredoc = false;
    node->execs = NULL;
    node->path = NULL;
    node->next = NULL;
    node->prev = NULL;
}

static t_executor *init_executor_list(int size)
{
    t_executor *list;
    t_executor *current;
    int         i;

    list = create_executor_list(size);
    if (!list)
        return (NULL);
    
    current = list;
    i = 0;
    while (current)
    {
        current->id = i++;
        current->size = size;
        set_executor_defaults(current);
        current = current->next;
    }
    return (list);
}

t_executor *prepare_executor(t_shell *shell)
{
    t_executor *list;
    int         size;

    size = get_size_executor(shell->lex_head);
    list = init_executor_list(size);
    if (!list)
        return (NULL);

    list = fill_executor_list(shell, list);
    if (!list)
        return (NULL);
    set_path_executor(list, shell->env);
    return (list);
}