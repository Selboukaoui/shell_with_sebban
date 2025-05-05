/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_cheker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:49:40 by asebban           #+#    #+#             */
/*   Updated: 2025/05/05 14:19:27 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_special(char x, int her)
{
    if (x == '\0')
        return (1);
    if (her == 69)
    {
        if (x  == '>' || x == '<' || x == '|')
            return (1);
    }
    else
    {
        if (x  == '>' || x == '|')
            return (1);
    }
    return (0);
}


// int check_quote_syntax(const char *input)
// {
//     int s;
//     int d;

//     s = 0;
//     d = 0;
//     while (*input)
//     {
//         if (*input == '\'')
//             s++;
//         else if (*input == '\"')
//             d++;
//         input++;
//     }
//     if (s % 2 == 0 && d % 2 == 0)
//         return (1);
//     else
//         return(0);
// }
int check_quote_syntax(const char *input)
{
    while (*input)
    {

        if (*input == '\'')
        {
            ++input;
            while (*input && *input != '\'')
                input++;
            if (*input == '\0')
                return (0);
        }
        else if (*input == '\"')
        {
            ++input;
            while (*input && *input != '\"')
                input++;
            if (*input == '\0')
                return (0); 
        }
        input++;
    }
    return(1);
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
                    return (write(2, "minishell: syntax error near unexpected token `newline'\n",57), 0);
                j++;
            }
            if (j > 2)
                return (0);
            while((*input == ' ' || *input == '\t') && *input)
                input++;
            if (ft_special(*input, 69))
                return (write(2, "minishell: syntax error !!\n",25), 0);
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
            while (*input && *input == '<')
            {
                input++;
                j++;
                if (*input == '\0')
                    break ;
            }
            while(*input && (*input == ' ' || *input == '\t'))
                input++;
            if (j > 2)
                return (write(2, "minishell: syntax error\n",25), 0);
            while(*input && (*input == ' ' || *input == '\t'))
                input++;
            if (ft_special(*input, 69))
                return (write(2, "minishell: syntax error\n", 25), 0);
            --input;
            if (j == 2)
            {
                // *H = 1;
                // check syntax error until you find speacial char      : )
                // check_quote_syntax_hdoc(input);
            }
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
    while (*input == ' ' || *input == '\t')
        input++;
    if (*input == '|')
        return (0);
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
            if (j > 1 || ft_special(*input, 123))
                return (0);
            j = 0;
            --input;
        }
        input++;
    }
    return (1);
}
