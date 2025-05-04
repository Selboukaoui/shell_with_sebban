/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_heap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:13:40 by asebban           #+#    #+#             */
/*   Updated: 2025/04/21 14:35:55 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_executor *free_executor_list(t_executor *list)
{
    t_executor *current;
    t_executor *next;

    if (!list)
        return (NULL);
    
    current = list;
    while (current)
    {
        next = current->next;
        if (current->fd_in != STDIN_FILENO)
            close(current->fd_in);
        if (current->fd_out != STDOUT_FILENO)
            close(current->fd_out);
        free_str_arr(current->path);
        free_str_arr(current->execs);
        //free(current);
        current = next;
    }
    return (NULL);
}

char *free_str_arr(char **arr)
{
    int i;

    if (!arr)
        return (NULL);
    
    i = 0;
    while (arr[i])
    {
        //free(arr[i]);
        i++;
    }
    //free(arr);
    return (NULL);
}

void free_environ(t_environ_list *env)
{
    t_environ_node *current;
    t_environ_node *next;

    if (!env)
        return;
    
    current = env->head;
    while (current)
    {
        next = current->next;
        //free(current->key);
        //free(current->value);
        //free(current);
        current = next;
    }
    env->head = NULL;
    env->size = 0;
}
