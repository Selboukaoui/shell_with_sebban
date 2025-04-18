/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_cheker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:49:40 by asebban           #+#    #+#             */
/*   Updated: 2025/04/18 14:22:56 by selbouka         ###   ########.fr       */
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
                return (0);
            while((*input == ' ' || *input == '\t') && *input)
                input++;
            if (!ft_isalpha(*input))
                return (0);
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
                return (0);
            while((*input == ' ' || *input == '\t') && *input) // !ft_isprint(*input) // expace mkytskipach
                input++; 
            if (!ft_isalpha(*input))
                return (0);
            j = 0;
        }
        input++;
    }
    return (1);
}


int check_pipe(char *input)
{
    int j;
    
    j = 0;
    while (*input)
    {
        if (*input == '|')
        {
            while (*input == '|' || *input == ' ' || *input == '\t')
            {
                if (*input  == '|')
                    j++;
                input++;
            }
            if (j > 1)
                return (0);
            j = 0;
            if (!ft_isalpha(*input))
                return (0);
        }
        input++;
    }
    return (1);
}