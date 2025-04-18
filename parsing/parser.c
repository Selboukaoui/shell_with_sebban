/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:27:05 by asebban           #+#    #+#             */
/*   Updated: 2025/04/19 10:59:16 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_lexer_list    *lexer(t_shell *shell)
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


int check_syntax(t_shell *input)
{
    if (!check_quote_syntax(input->rl_copy))
        return (ft_putstr_fd("syntax error near unexpected token`(quote)'\n", 2), 1);
    if (!check_parenthesis(input->rl_copy))
        return (ft_putstr_fd("syntax error near unexpected token`(parent)'\n", 2), 1);
    if (!check_redirect_in(input->rl_copy) || !check_redirect_out(input->rl_copy))
        return (ft_putstr_fd("syntax error near unexpected token`(redirect)'\n", 2), 1);
    if (!check_pipe(input->rl_copy))
        return (ft_putstr_fd("syntax error near unexpected token`(|)'\n", 2), 1);
    return (1);
    
}

bool    parser(t_shell *shell)
{

    
    // if (!check_syntax(shell))
    //     return (0);
    
    // clean excutor list
    // if (shell->executor)
    // {
    //     shell->executor = free_executor_list(shell->executor);
    //     shell->executor = NULL;
    // }
    
    shell->lex_head = lexer(shell);
    if (!shell->lex_head)
        return (false);
    
    shell->executor = prepare_executor(shell);
    return (true);
}
