/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_hard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:26:18 by asebban           #+#    #+#             */
/*   Updated: 2025/05/09 16:16:04 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool check_if_first_is_export(char *str)
{
    int i = 0;
    if (!str)
        return false;
    while (str[i] && (str[i] == ' ' || str[i] == '\t'))
        i++;
    if (strncmp(str + i, "export", 6) != 0)
        return false;
    i += 6;
    return (str[i] == '\0' || str[i] == ' ' || str[i] == '\t');
}


static int calculate_max_len(const char *str, t_shell *shell)
{
    int i = 0;
    int len = 0;
    bool in_single = false;
    bool in_double = false;

    while (str && str[i]) {
        if (str[i] == '\'' && !in_double) {
            in_single = !in_single;
            len++; i++;
        }
        else if (str[i] == '"' && !in_single) {
            in_double = !in_double;
            len++; i++;
        }
        else if (str[i] == '$' && !in_single && str[i + 1] && (isalnum(str[i+1]) || str[i+1] == '_')) {
            i++; // skip $
            char var_name[256];
            int k = 0;
            while ((isalnum(str[i]) || str[i] == '_') && k < 255)
                var_name[k++] = str[i++];
            var_name[k] = '\0';
            char *value = get_env_value(shell->env, var_name);
            if (value)
                len += strlen(value);
        }
        else {
            len++;
            i++;
        }
    }
    return len;
}


char *change_all_var(const char *str, t_shell *shell)
{
    int i = 0, j = 0;
    char *output;
    int len;
    char var_name[256];
    char *value;
    bool in_single = false;
    bool in_double = false;

    if (!str)
        return NULL;

    len = calculate_max_len(str, shell);
    output = ft_malloc(len + 1, 1);
    if (!output)
        return NULL;

    while (str[i]) {
        if (str[i] == '\'' && !in_double) {
            in_single = !in_single;
            output[j++] = str[i++];
        }
        else if (str[i] == '"' && !in_single) {
            in_double = !in_double;
            output[j++] = str[i++];
        }
        else if (str[i] == '$' && !in_single && str[i+1] && (ft_isalnum(str[i+1]) || str[i+1] == '_')) {
            i++; // skip $
            int k = 0;
            while ((ft_isalnum(str[i]) || str[i] == '_') && k < 255)
                var_name[k++] = str[i++];
            var_name[k] = '\0';
            value = get_env_value(shell->env, var_name);
            if (value) {
                // Only wrap quotes if output is currently not inside any quote context
                bool wrap = !in_double;
                if (wrap)
                    output[j++] = '"';
                for (char *p = value; *p; ++p)
                    output[j++] = *p;
                if (wrap)
                    output[j++] = '"';
            }
        }
        else {
            output[j++] = str[i++];
        }
    }
    output[j] = '\0';
    return output;
}

// char *change_all_var(char *str, t_shell *shell)
// {
//     int i = 0, j = 0;
//     char *output;
//     int len;
//     char var_name[256];
//     char *value;
//     bool first_var = true; // Flag to track the first variable

//     if (!str)
//         return (NULL);

//     len = calculate_max_len(str, shell); // Your function to estimate enough space
//     output = ft_malloc(len + 1, 1); // Your custom malloc
//     if (!output)
//         return (NULL);

//     while (str[i])
//     {
//         if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
//         {
//             int k = 0;
//             i++; // Skip the $
//             while ((ft_isalnum(str[i]) || str[i] == '_') && k < 255)
//                 var_name[k++] = str[i++];
//             var_name[k] = '\0';

//             value = get_env_value(shell->env, var_name); // Get env var value
//             if (value)
//             {
//                 if (!first_var)
//                 {
//                     output[j++] = '"'; // Add opening quote
//                 }

//                 while (*value)
//                     output[j++] = *value++;

//                 if (!first_var)
//                 {
//                     output[j++] = '"'; // Add closing quote
//                 }
//             }
//         }
//         else
//             output[j++] = str[i++];
        
//         first_var = false; // After the first variable, set the flag to false
//     }
//     output[j] = '\0';
//     return (output);
// }


char *join_strings(char **strings, char delimiter) {
    if (!strings || !*strings) return NULL;

    size_t total_length = 0;
    size_t count = 0;

    // Calculate total length required
    for (size_t i = 0; strings[i]; i++) {
        total_length += strlen(strings[i]);
        count++;
    }

    // Add space for delimiters and null terminator
    total_length += (count - 1) + 1;

    char *result = malloc(total_length);
    if (!result) return NULL;

    result[0] = '\0';

    for (size_t i = 0; i < count; i++) {
        strcat(result, strings[i]);
        if (i < count - 1) {
            size_t len = strlen(result);
            result[len] = delimiter;
            result[len + 1] = '\0';
        }
    }

    return result;
}

char *export_hard(char *str, t_shell *shell)
{
    char **arr = ft_split1(str, '|');
    int i = 0;
    char *result;

    if (!arr)
        return NULL;

    while (arr[i]) {
        if (check_if_first_is_export(arr[i])) {
            char *expanded = change_all_var(arr[i], shell);
            if (expanded) {
                arr[i] = expanded;
            }
        }
        i++;
    }

    // Rejoin with '|'
    // (Implement join_strings as before or inline concatenation)
    result = join_strings(arr, '|');
    return result;
}

