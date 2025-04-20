/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_cheker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:49:40 by asebban           #+#    #+#             */
/*   Updated: 2025/04/19 15:09:47 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int ft_special(char x)
{
    if (x  == '>' || x == '<' || x == '|')
        return (1);
    return (0);
}

int check_quote_syntax(const char *input)
{
    int s;
    int d;

    s = 0;
    d = 0;
    while (*input)
    {
        // if (*input == '\\')
        // {
            
        // }
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
                if (*input == '\0')
                    return (0);
                j++;
            }
            if (j > 2)
                return (0);
            while((*input == ' ' || *input == '\t') && *input)
                input++;
            if (ft_special(*input))
                return (0);
            j = 0;
            --input;

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
                if (*input == '\0')
                    return (0);
                j++;
            }
            while((*input == ' ' || *input == '\t') && *input) // !ft_isprint(*input) // space mkytskipach
                input++; 
            if (j > 2 || ft_special(*input))
                return (0);
            j = 0;
            --input;

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
            while ((*input == '|' || *input == ' ' || *input == '\t') && *input)
            {
                if (*input  == '|')
                    j++;
                input++;
                if (*input == '\0')
                    return (0);
            }
            if (j > 1 || ft_special(*input))
                return (0);
            j = 0;
            --input;
        }
        input++;
    }
    return (1);
}
