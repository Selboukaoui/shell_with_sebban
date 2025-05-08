/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:08:16 by asebban           #+#    #+#             */
/*   Updated: 2025/05/08 22:45:24 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int should_split_token(t_lexer_list *token)
{
    if (!token->str)
        return (0);
    if (!ft_strchr(token->str, ' '))
        return (0);
    return (1);
}

/**
 * Insert new tokens for parts[1..] immediately after `token` in the list.
 */
static void create_new_tokens(t_lexer_list *token, char **parts)
{
    t_lexer_list *curr = token;
    t_lexer_list *new_tok;
    int           i = 1;

    while (parts[i])
    {
        if (!ft_strchr(parts[i], ' '))
        {
            new_tok = create_token(ft_strdup(parts[i]));
            new_tok->next = curr->next;
            if (curr->next)
                curr->next->prev = new_tok;
            curr->next = new_tok;
            new_tok->prev = curr;
            curr = new_tok;
        }
        i++;
    }
}

char *handle_special_chars(t_shell *obj, char *str)
{
    char *value = NULL;

    if (!str)
        return (NULL);

    /* "$?" → last exit code */
    if (str[0] == '?' && str[1] == '\0')
    {
        value = ft_itoa(exit_status(0,0));
    }
    /* "$$" → literal $ */
    else if (str[0] == '$' && str[1] == '\0')
    {
        value = ft_strdup("$");
    }
    /* other single-character specials */
    else if ((str[0] == '%' || str[0] == '.') && str[1] == '\0')
    {
        value = get_special_value(obj, str);
    }
    /* not a special case */
    return (value);
}

/**
 * Split token->str on spaces, update the original token to the first word,
 * and chain newly created tokens for the remaining words.
 */
void split_expanded(t_lexer_list *token)
{
    char **parts;

    if (!should_split_token(token))
        return ;
    parts = ft_split(token->str, ' ');
    if (!parts)
        return ;
    // free(token->str);
    token->str = ft_strdup(parts[0]);
    create_new_tokens(token, parts);
    // free_argv(parts);
}

/* expand.c */

/**
 * Fetch the variable’s value: handle special chars ($?, $$),
 * then fall back to environment lookup, then empty string.
 */
char *get_value(t_shell *obj, char *str)
{
    char *value;

    value = handle_special_chars(obj, str);
    if (value == NULL)
        value = get_env_value(obj->env, str);
    if (value == NULL)
        value = ft_strdup("");
    return (value);
}

/**
 * Walk the entire lexer list in the shell, expand each token unless
 * we’re in a here-doc context (“<<”).
 */
void expand(t_shell *obj)
{
    t_lexer_list *token = obj->lex_head;
    int           j = 0;

    while (token)
    {
        if (token->str && ft_strcmp(token->str, "<<") == 0)
        {
            handle_special_tokens(&token);
            continue ;
        }
        process_token(obj, token, &j);
        token = token->next;
    }
}