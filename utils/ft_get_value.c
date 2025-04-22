/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:33:32 by asebban           #+#    #+#             */
/*   Updated: 2025/04/22 10:23:08 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\v' || c == '\f' || c == '\r');
}

static char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i = 0;

    while (i < n && src[i])
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

static void int_to_str(int num, char *str)
{
    int i;
    int j;
    int is_negative;

    j = 0;
    i = 0;
    is_negative = 0;
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }
    while (num != 0)
    {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative)
        str[i++] = '-';
    str[i] = '\0';

    while(j < i / 2)
    {
        char temp;
        temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
        j++;
    }
}

char *get_env_value(t_environ_list *env_list, char *key)
{
    t_environ_node *curr = env_list->head;

    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}


char *replace_vars(char *input, t_shell *shell)
{
    size_t  i;
    size_t  j;
    size_t  len;
    char    *output;
    int     k;
    int     in_single_quote;
    int     in_double_quote;

    in_double_quote = 0;
    in_single_quote = 0;
    i = 0;
    j = 0;
    k = 0;
    len = ft_strlen(input);
    output = malloc(PATH_MAX + 1); // replace with ft_malloc
    if (!output)
        return NULL;

    while (i < len)
    {
        if (input[i] == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            output[j++] = input[i++];
        }
        else if (input[i] == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            output[j++] = input[i++];
        }
        else if (input[i] == '$' && input[i + 1] == '?' && !in_single_quote)
        {
            int status = exit_status(0, 0);
            char status_str[12];
            int_to_str(status, status_str);
            while (status_str[k])
                output[j++] = status_str[k++];
            i += 2;
        }
        // Handle $VARNAME
        else if (input[i] == '$' && ft_isalpha(input[i + 1]) && !in_single_quote)
        {
            int var_start = i + 1;
            int var_len = 0;

            while (ft_isalnum(input[var_start + var_len]) || input[var_start + var_len] == '_')
                var_len++;

            char var_name[256];
            ft_strncpy(var_name, &input[var_start], var_len);
            var_name[var_len] = '\0';

            char *env_value = get_env_value(shell->env, var_name);
            if (env_value)
            {
                k = 0;
                while (env_value[k])
                output[j++] = env_value[k++];
            }
            else
            {
                size_t tmp = i + var_len + 1;
                int only_spaces_after = 1;
            
                while (input[tmp])
                {
                    if (!ft_isspace(input[tmp]))
                    {
                        only_spaces_after = 0;
                        break;
                    }
                    tmp++;
                }
            
                if (only_spaces_after)
                    output[j++] = '\n';
                // else: do nothing (just remove the missing $VAR silently)
            }
            
            i += var_len + 1;
        }
        // If $ inside single quotes, treat as normal char
        else
        {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
    return output;
}