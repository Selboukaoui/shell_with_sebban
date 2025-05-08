/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:13:00 by asebban           #+#    #+#             */
/*   Updated: 2025/05/08 22:24:35 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *no_quotes(t_shell *obj, char **argv, int *i, int *j)
{
    char            *str = NULL;
    int              x   = 0;

    while (argv[*i] && ft_strcmp(argv[*i], "\"") && ft_strcmp(argv[*i], "\'"))
    {
        if (ft_strchr(argv[*i], '$'))
        {
            if (ft_strchr(argv[*i], '='))
                x++;
            /* skip the '$' */
            str = ft_strjoin2(str, get_value(obj, argv[*i] + 1), 2);
            (*j)++;
            if (x > 0)
                str = ft_strjoin2(str, "=", 1);
            x = 0;
        }
        else
            str = ft_strjoin2(str, argv[*i], 1);
        (*i)++;
    }
    return (str);
}

char *do_quotes(t_shell *obj, char **argv, int *i, int *j)
{
    char *str = NULL;

    (*i)++;  /* skip the opening quote */
    while (argv[*i] && ft_strcmp(argv[*i], "\""))
    {
        if (ft_strchr(argv[*i], '$'))
        {
            str = ft_strjoin2(str, get_value(obj, argv[*i] + 1), 2);
            (*j)++;
        }
        else
            str = ft_strjoin2(str, argv[*i], 1);
        (*i)++;
    }
    return (str);
}

char *si_quotes(char **argv, int *i)
{
    char *str = NULL;

    (*i)++;  /* skip the opening single quote */
    while (argv[*i] && ft_strcmp(argv[*i], "\'"))
    {
        str = ft_strjoin2(str, argv[*i], 1);
        (*i)++;
    }
    return (str);
}

t_lexer_list *create_token(char *str)
{
    t_lexer_list *tok;

    tok = malloc(sizeof(t_lexer_list));
    if (!tok)
        return (NULL);
    tok->str  = str;
    tok->type = CMD;     /* or whatever default makes sense */
    tok->prev = NULL;
    tok->next = NULL;
    return (tok);
}

char *get_special_value(t_shell *obj, char *str)
{
    char *value = NULL;
    (void)obj;
    if (*str == '\0')
        value = ft_strdup("$");
    else if (*str == '%')
        value = ft_strdup("$%");
    else if (*str == '?')
        value = ft_itoa(exit_status(0,0));
    else if (*str == '.')
        value = ft_strdup("$.");
    return (value);
}