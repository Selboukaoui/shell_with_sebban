/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:27:05 by asebban           #+#    #+#             */
/*   Updated: 2025/05/08 22:51:15 by asebban          ###   ########.fr       */
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

int check_syntax(t_shell *input)
{

    if (!check_redirect_in(input->rl_copy) || !check_redirect_out(input->rl_copy))
    {
        exit_status(EXIT_SET, 2);
        return (0);
    }
    if (!check_quote_syntax(input->rl_copy))
    {
        ft_putstr_fd("minishell:  syntax error !!\n", 2);
        exit_status(EXIT_SET, 2);
        return (0);
        
    }
    if (!check_pipe(input->rl_copy))
    {
        ft_putstr_fd("minishell:  syntax error near unexpected token `|'\n", 2);
        exit_status(EXIT_SET, 2);
        return (0);
    }
    return (1);
}


bool    parser(t_shell *shell)
{

    
    if (!check_syntax(shell))
        return (false);
    
    // clean excutor list
    // if (shell->executor)
    // {
    //     shell->executor = free_executor_list(shell->executor);
    //     shell->executor = NULL;
    // }
    
    shell->lex_head = lexer(shell);
    if (!shell->lex_head)
        return (false);
    t_lexer_list *lexr = shell->lex_head;
    while(lexr)
    {
            printf("str----->%s\n", lexr->str);
            printf("type----->%u\n",lexr->type);
            printf("-----------------------\n");
            lexr = lexr->next;
    }
    expand(shell);
    shell->executor = prepare_executor(shell);
    return (true);
}

