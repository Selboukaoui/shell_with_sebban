/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_cheker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:49:40 by asebban           #+#    #+#             */
/*   Updated: 2025/04/28 18:14:31 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_special(char x, int her)
{
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
    // int open;
    // int close;
    // int state;

    // state = 0;
    // open = 0;
    // close = 0;
    while (*input)
    {
        
        if (*input == '\'' || *input == '\"')
        {
            ++input;
            while ((*input != '\'' && *input != '\"') && *input)
                ++input;
            ++input;
        }
        if (*input == '(' /*&& state % 2 != 0*/ )
            return (ft_putstr_fd("syntax error near unexpected token `('\n", 2), 0);
        else if (*input == ')' /*&& state % 2 != 0*/)
            return (ft_putstr_fd("syntax error near unexpected token `)'\n", 2), 0);
            // state++;
        // else
        //     break ;
        input++;
    }
    // if (open < close)
    // {
    //     ft_putstr_fd("syntax error near unexpected token `('\n", 2);
    //     return (0);
    // }
    // else if (open > close)
    // {
    //     ft_putstr_fd("syntax error near unexpected token `)'\n", 2);
    //     return (0);
    // }
    // else
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
                    return (0);
                j++;
            }
            if (j > 2)
                return (0);
            while((*input == ' ' || *input == '\t') && *input)
                input++;
            if (ft_special(*input, 69))
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
            if (j > 2 || ft_special(*input, 69))
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



// #include "../includes/minishell.h"

// int ft_special(char x, int her)
// {
//     // if (!x)
//     //     return (0);
//     // printf ("**%c**\n", x);
//     if (her == 69)
//     {
//         if (x  == '>' || x == '<' || x == '|' || x == '\0')
//             return (1);
//     }
//     else
//     {
//         if (x  == '>' || x == '|')
//             return (1);
//     }
//     return (0);
// }

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


// int check_parenthesis(char *input)
// {
//     // int open;
//     // int close;
//     // int state;

//     // state = 0;
//     // open = 0;
//     // close = 0;
//     while (*input)
//     {
        
//         if (*input == '\'' || *input == '\"')
//         {
//             ++input;
//             while ((*input != '\'' && *input != '\"') && *input)
//                 ++input;
//             ++input;
//         }
//         if (*input == '(' /*&& state % 2 != 0*/ )
//             return (ft_putstr_fd("syntax error near unexpected token `('\n", 2), 0);
//         else if (*input == ')' /*&& state % 2 != 0*/)
//             return (ft_putstr_fd("syntax error near unexpected token `)'\n", 2), 0);
//             // state++;
//         // else
//         //     break ;
//         input++;
//     }
//     // if (open < close)
//     // {
//     //     ft_putstr_fd("syntax error near unexpected token `('\n", 2);
//     //     return (0);
//     // }
//     // else if (open > close)
//     // {
//     //     ft_putstr_fd("syntax error near unexpected token `)'\n", 2);
//     //     return (0);
//     // }
//     // else
//         return(1);
// }

// int check_redirect_out(char *input)
// {
//     int j;
    
//     j = 0;
//     while (*input)
//     {
//         if (*input == '>')
//         {
//             while (*input == '>')
//             {
//                 input++;
//                 if (*input == '\0')
//                     return (0);
//                 j++;
//             }
//             if (j > 2)
//                 return (0);
//             while((*input == ' ' || *input == '\t') && *input)
//                 input++;
//             if (ft_special(*input, 69))
//                 return (0);
//             j = 0;
//             --input;

//         }
//         input++;
//     }
//     return (1);
// }

// int check_redirect_in(char *input)
// {
//     int j;

//     j = 0;
//     while (*input)
//     {
//         if (*input == '<')
//         {
//             while (*input && *input == '<')
//             {
//                 input++;
//                 // if (*input == '\0')
//                 //     break ;
//                 j++;
//             }
//             // printf ("%c", *input);
//             while(*input && (*input == ' ' || *input == '\t'))
//             {
//                 // printf ("%c", *input);
//                 input++;
//             }
//             if (ft_special(*input, 69) && j < 4)
//             {
//                 return (printf("minishell:  syntax error near unexpected token `%c'\n", *input), 0);
//             }
//             if (j > 3 && j < 5)
//                 return (printf("minishell:  syntax error near unexpected token `<'\n"), 0);
//             else if (j > 4 && j < 6)
//                 return (printf("minishell:  syntax error near unexpected token `<<'\n"), 0);
//             else if (j > 5)
//                 return (printf("minishell:  syntax error near unexpected token `<<'\n"), 0);
//             j = 0;
//             --input;
//         }
//         input++;
//     }
//     return (1);
// }


// int check_pipe(char *input)
// {
//     int j;
    
//     j = 0;
//     while (*input == ' ' || *input == '\t')
//         input++;
//     if (*input == '|')
//         return (0);
//     while (*input)
//     {
//         if (*input == '|')
//         {
//             while ((*input == '|' || *input == ' ' || *input == '\t') && *input)
//             {
//                 if (*input  == '|')
//                     j++;
//                 input++;
//                 if (*input == '\0')
//                     return (0);
//             }
//             if (j > 1 || ft_special(*input, 123))
//                 return (0);
//             j = 0;
//             --input;
//         }
//         input++;
//     }
//     return (1);
// }
