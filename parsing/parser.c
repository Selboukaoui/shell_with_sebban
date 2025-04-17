/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:27:05 by asebban           #+#    #+#             */
/*   Updated: 2025/04/17 12:34:30 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_lexer_list    *lexer(t_shell *shell)
{
    //clean
    // if (shell->lex_head)
    // {
    //     shell->lex_head = free_lex(shell->lex_head);
    //     shell->lex_head = NULL;
    // }
    
    //handle NULL in input
    if (!shell->rl_copy)
        return (NULL);
    shell->lex_head = tokenize(shell);
    if (!shell->lex_head)
        return (NULL);
    // if (!check_pipe_syntax(shell->lex_head, shell))
    // {
    //     shell->lex_head = free_lex(shell->lex_head);
    //     return (NULL);
    // }
    return (shell->lex_head);
}

bool    parser(t_shell *shell)
{
    // clean excutor list
    // if (shell->executor)
    // {
    //     shell->executor = free_executor_list(shell->executor);
    //     shell->executor = NULL;
    // }
    shell->lex_head = lexer(shell);
}