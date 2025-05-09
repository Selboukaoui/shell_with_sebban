/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:08:16 by asebban           #+#    #+#             */
/*   Updated: 2025/05/09 12:45:48 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

static char *ft_strtrim1(const char *s1) {
    return ft_strtrim(s1, " \t\n");
}

static char  *get_env_value2(const char *key, t_shell *shell)
{
    t_environ_node *node;
    char           *copy;

    if (!key || !shell || !shell->env)
        return (ft_strdup(""));
    node = shell->env->head;
    while (node)
    {
        if (ft_strcmp(node->key, key) == 0)
        {
            copy = ft_strdup(node->value);
            return (copy ? copy : ft_strdup(""));
        }
        node = node->next;
    }
    return (ft_strdup(""));
}

static int   skip_heredoc(const char *in, char **res, int *i)
{
    char *tmp;

    tmp = ft_strjoin(*res, "<<");
    free(*res);
    *res = tmp;
    *i += 2;
    while (in[*i] && ft_isspace(in[*i]))
    {
        tmp = ft_strjoin(*res, ft_substr(in, *i, 1));
        free(*res);
        *res = tmp;
        (*i)++;
    }
    return (1);
}

static int   expand_variable(const char *in, char **res,
                              int *i, t_shell *shell)
{
    char    *tmp;
    char    *val;
    int      start;
    int      len;

    if (in[*i + 1] == '?')
    {
        val = ft_itoa(exit_status(EXIT_GET, 0));
        tmp = ft_strjoin(*res, val);
        free(*res);
        *res = tmp;
        free(val);
        *i += 2;
        return (1);
    }
    start = *i + 1;
    len = 0;
    while (ft_isalnum(in[start + len]) || in[start + len] == '_')
        len++;
    val = get_env_value2(ft_substr(in, start, len), shell);
    tmp = ft_strjoin(*res, val);
    free(*res);
    *res = tmp;
    free(val);
    *i = start + len;
    return (1);
}

static int   expand_home(const char *in, char **res, int *i, t_shell *shell)
{
    char *val;
    char *tmp;

    (void)in;
    val = get_env_value2("HOME", shell);
    tmp = ft_strjoin(*res, val);
    free(*res);
    *res = tmp;
    free(val);
    (*i)++;
    return (1);
}

/**
 * Performs variable and home expansions on an input segment.
 */
static char *segment_expand(const char *segment, t_shell *shell)
{
    char    *res;
    char    *tmp;
    int      i;
    int      in_squote;
    int      skip_word;

    res = ft_strdup("");
    i = 0;
    in_squote = 0;
    skip_word = 0;
    while (segment[i])
    {
        if (segment[i] == '\'' && !skip_word)
            in_squote = !in_squote;
        if (!in_squote && segment[i] == '<' && segment[i+1] == '<')
            skip_word = skip_heredoc(segment, &res, &i);
        else if (skip_word)
        {
            if (ft_isspace(segment[i]))
                skip_word = 0;
            tmp = ft_strjoin(res, ft_substr(segment, i, 1));
            free(res);
            res = tmp;
            i++;
        }
        else if (!in_squote && segment[i] == '$')
            expand_variable(segment, &res, &i, shell);
        else if (!in_squote && segment[i] == '~' && i == 0
                 && (segment[1] == '/' || segment[1] == '\0'))
            expand_home(segment, &res, &i, shell);
        else
        {
            tmp = ft_strjoin(res, ft_substr(segment, i, 1));
            free(res);
            res = tmp;
            i++;
        }
    }
    return (res);
}

/**
 * Processes an export segment like "export a=$a" and wraps the value in quotes.
 * If the segment does not start with "export ", returns a strdup of original.
 */
static char *process_export_segment(const char *seg, t_shell *shell)
{
    char **parts;
    char *ret;
    char *value_expanded;

    if (ft_strncmp(seg, "export ", 7) != 0)
        return (ft_strdup(seg));

    parts = ft_split(seg + 7, '=');
    if (!parts || !parts[0])
    {
        // Handle cases like 'export VAR' without assignment
        // ft_free_split(parts);
        return (ft_strdup(seg));
    }

    if (!parts[1])
    {
        // Handle cases like 'export VAR=' with empty value
        ret = ft_strjoin("export ", parts[0]);
        ret = ft_strjoin(ret, "=\"\"");
        // ft_free_split(parts);
        return (ret);
    }

    value_expanded = segment_expand(parts[1], shell);
    ret = ft_strjoin("export ", parts[0]);
    ret = ft_strjoin(ret, "=\"");
    ret = ft_strjoin(ret, value_expanded);
    ret = ft_strjoin(ret, "\"");
    // free(value_expanded);
    // ft_free_split(parts);
    return (ret);
}

/**
 * Splits an input by '|' and processes export segments,
 * then reassembles with ' | '.
 */
char *expand_variables(const char *input, t_shell *shell)
{
    char  **segs;
    char  *joined;
    char  *tmp;
    int    i;

    segs = ft_split(input, '|');
    if (!segs)
        return (ft_strdup(input));
    joined = NULL;
    i = 0;
    while (segs[i])
    {
        tmp = process_export_segment(ft_strtrim1(segs[i]), shell);
        if (joined)
        {
            joined = ft_strjoin(joined, " | ");
            joined = ft_strjoin(joined, tmp);
        }
        else
            joined = ft_strdup(tmp);
        free(tmp);
        i++;
    }
    // ft_free_split(segs);
    return (joined);
}

char *replace_vars1(const char *input, t_shell *shell)
{
    char    *result;
    char    *tmp;
    int     i;
    int     in_squote;
    int     skip_word;
    int     in_export;
    int     in_assignment;

    result = ft_strdup("");
    i = 0;
    in_squote = 0;
    skip_word = 0;
    in_export = 0;
    in_assignment = 0;

    /* Detect if command starts with "export " */
    if (ft_strncmp(input, "export ", 7) == 0)
        in_export = 1;

    while (input[i])
    {
        /* Handle single quotes (no expansion inside) */
        if (input[i] == '\'' && !skip_word)
            in_squote = !in_squote;
        
        /* On export, after '=', enter assignment mode until space */
        if (in_export && !in_assignment && input[i] == '=')
        {
            in_assignment = 1;
            tmp = ft_strjoin(result, "=");
            free(result);
            result = tmp;
            i++;
            continue;
        }

        /* If inside an assignment and encounter double-quoted value */
        if (in_assignment && input[i] == '"')
        {
            /* Preserve opening quote */
            tmp = ft_strjoin(result, "\"");
            free(result);
            result = tmp;
            i++;
            /* Copy everything literally until closing quote */
            while (input[i] && input[i] != '"')
            {
                tmp = ft_strjoin(result, ft_substr(input, i, 1));
                free(result);
                result = tmp;
                i++;
            }
            /* Preserve closing quote */
            if (input[i] == '"')
            {
                tmp = ft_strjoin(result, "\"");
                free(result);
                result = tmp;
                i++;
            }
            /* End assignment on space or end-of-input */
            if (input[i] == ' ' || input[i] == '\0')
                in_assignment = 0;
            continue;
        }

        /* Default heredoc, skip, variable and home-handling preserved */
        if (!in_squote && input[i] == '<' && input[i + 1] == '<')
            skip_word = skip_heredoc(input, &result, &i);
        else if (skip_word)
        {
            if (ft_isspace(input[i]))
                skip_word = 0;
            tmp = ft_strjoin(result, ft_substr(input, i, 1));
            free(result);
            result = tmp;
            i++;
        }
        /* Handle variable expansion only when valid name/char follows */
        else if (!in_squote && input[i] == '$'
                 && (input[i+1] == '?' || ft_isalnum(input[i+1]) || input[i+1] == '_'))
        {
            expand_variable(input, &result, &i, shell);
        }
        /* Otherwise treat '$' literally */
        else if (!in_squote && input[i] == '$')
        {
            tmp = ft_strjoin(result, "$");
            free(result);
            result = tmp;
            i++;
        }
        else if (!in_squote && input[i] == '~' && i == 0
                 && (input[1] == '/' || input[1] == '\0'))
            expand_home(input, &result, &i, shell);
        else
        {
            tmp = ft_strjoin(result, ft_substr(input, i, 1));
            free(result);
            result = tmp;
            i++;
        }
    }
    return (result);
}

/* The helper functions skip_heredoc, expand_variable, expand_home remain unchanged */
