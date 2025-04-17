/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:04 by asebban           #+#    #+#             */
/*   Updated: 2025/04/17 12:38:54 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_lexer_list *tokenize(t_shell *shell)
{
    char            **input_array;
    t_lexer_list    *lexer_list;

    if (!shell || !shell->rl_copy)
        return (NULL);
    input_array = ft_newsplit(shell->rl_copy);
    if (!input_array)
        return (NULL);
    
    // lexer_list = fill_lexer_list(input_array);
    // free_str_arr(input_array);
    return (lexer_list);
}