/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_cheker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:49:40 by asebban           #+#    #+#             */
/*   Updated: 2025/04/17 21:09:10 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int check_quote_syntax(const char *input)
{
    int s;
    int d;

    s = 0;
    d = 0;
    while (*input)
    {
        if (*input == '\'')
            s++;
        else if (*input == '\"')
            d++;
        input++;
    }
    if (s % 2 == 0 && d % 2 == 0)
        return (1);
    else
        return(0);
}


int check_parenthesis(char *input)
{
    int open;
    int close;
    
    open = 0;
    close = 0;
    while (*input)
    {
        if (*input == '(')
            open++;
        else if (*input == ')')
            close++;
        input++;
    }
    if (open == close)
        return (1);
    else
        return(0);
}

int check_redirect_out(char *input)
{
    int j;
    
    j = 0;
    while (*input)
    {
        if (*input == '>')
        {
            while (*input == '>')
            {
                input++;
                j++;
            }
            if (j > 2)
                return (ft_putstr_fd("Error\nra 2 {>>} ashbi\n synatx checker line 58\n", 2),0);
            while(!ft_isprint(*input))
                input++;
            if (!ft_isalpha(*input))
                return (ft_putstr_fd("Error\nchi 7aja tma\nsynatx checker line 58\n", 2),0);
            j = 0;
        }
        input++;
    }
    return (1);

}

int check_redirect_in(char *input)
{
    int j;
    
    j = 0;
    while (*input)
    {
        if (*input == '<')
        {
            while (*input == '<')
            {
                input++;
                j++;
            }
            if (j > 2)
                return (ft_putstr_fd("Error\nra 2 {<} ashbi\n synatx checker line 58\n", 2),0);
            while(!ft_isprint(*input) && *input)
                input++;
            printf ("\n ** %c ** \n", *input);
            if (!ft_isalpha(*input) && *input)
                return (ft_putstr_fd("Error\nchi 7aja tma\nsynatx checker line 58\n", 2),0);
            j = 0;
        }
        input++;
    }
    return (1);
}


