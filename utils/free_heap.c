/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_heap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:13:40 by asebban           #+#    #+#             */
/*   Updated: 2025/04/16 15:14:06 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_environ(t_environ_list *environ)
{
    t_environ_node *current;
    t_environ_node *next;

    if (!environ)
        return;
    
    current = environ->head;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    environ->head = NULL;
    environ->size = 0;
}
