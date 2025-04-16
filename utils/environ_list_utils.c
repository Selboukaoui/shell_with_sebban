/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_list_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:02:22 by asebban           #+#    #+#             */
/*   Updated: 2025/04/16 15:03:42 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environ_node *create_environ_node(char *var)
{
    t_environ_node  *new_node;
    char            *equal_sign;
    size_t          key_len;

    if (!var)
        return (NULL);
    
    new_node = (t_environ_node *)ft_calloc(1, sizeof(t_environ_node));
    if (!new_node)
        return (NULL);

    equal_sign = ft_strchr(var, '=');
    if (equal_sign)
    {
        key_len = equal_sign - var;
        new_node->key = ft_substr(var, 0, key_len);
        new_node->value = ft_strdup(equal_sign + 1);
    }
    else
    {
        new_node->key = ft_strdup(var);
        new_node->value = NULL;
    }

    if (!new_node->key || (equal_sign && !new_node->value))
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return (NULL);
    }

    new_node->next = NULL;
    return (new_node);
}

t_environ_node *add_back_environ_node(t_environ_list *environ, t_environ_node *node)
{
    t_environ_node *current;

    if (!environ || !node)
        return (NULL);

    if (!environ->head)
    {
        environ->head = node;
        environ->size = 1;
        return (node);
    }

    current = environ->head;
    while (current->next)
        current = current->next;

    current->next = node;
    environ->size++;
    return (node);
}