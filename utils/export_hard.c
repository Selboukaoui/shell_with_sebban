/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_hard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:26:18 by asebban           #+#    #+#             */
/*   Updated: 2025/05/09 14:03:19 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	check_if_first_is_export(char *str)
{
	int	i = 0;

	if (!str)
		return (false);
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (ft_strncmp(str + i, "export", 6) != 0)
		return (false);
	i += 6;
	if (str[i] == '\0' || str[i] == ' ' || str[i] == '\t')
		return (true);

	return (false);
}


static int calculate_max_len(const char *str, t_shell *shell)
{
    int i = 0;
    int len = 0;

    if (!str)
        return 0;

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && (isalnum(str[i + 1]) || str[i + 1] == '_'))
        {
            i++; // Skip the '$'
            char var_name[256];
            int k = 0;

            // Extract variable name
            while ((isalnum(str[i]) || str[i] == '_') && k < 255)
                var_name[k++] = str[i++];
            var_name[k] = '\0';

            // Get environment variable value
            char *value = get_env_value(shell->env, var_name);
            if (value)
                len += strlen(value) + 2; // Add 2 for the surrounding quotes
        }
        else
        {
            len++;
            i++;
        }
    }

    return len;
}


char *change_all_var(char *str, t_shell *shell)
{
    int i = 0, j = 0;
    char *output;
    int len;
    char var_name[256];
    char *value;
    bool first_var = true; // Flag to track the first variable

    if (!str)
        return (NULL);

    len = calculate_max_len(str, shell); // Estimate the required space
    output = ft_malloc(len + 1, 1); // Allocate memory
    if (!output)
        return (NULL);

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
        {
            int k = 0;
            i++; // Skip the $
            while ((ft_isalnum(str[i]) || str[i] == '_') && k < 255)
                var_name[k++] = str[i++];
            var_name[k] = '\0';

            value = get_env_value(shell->env, var_name); // Get environment variable value
            if (value)
            {
                if (!first_var)
                {
                    output[j++] = '"'; // Add opening quote
                }

                while (*value)
                    output[j++] = *value++;

                if (!first_var)
                {
                    output[j++] = '"'; // Add closing quote
                }
            }
        }
        else
            output[j++] = str[i++];
        
        first_var = false; // After the first variable, set the flag to false
    }
    output[j] = '\0';
    return (output);
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
    char    **arr;
    int     i;
    char    *result;

    i = 0;
    arr = ft_split1(str, '|');
    if (!arr)
        return (NULL);

    while (arr[i])
    {
        if (check_if_first_is_export(arr[i]))
        {
            char *expanded = change_all_var(arr[i], shell);
            if (expanded)
            {
                // free(arr[i]); // Free old string
                arr[i] = expanded;
            }
        }
        i++;
    }

    // Combine arr back into a single string
    result = join_strings(arr, '|'); // Implement join_array to concatenate strings with '|'
    // free_array(arr); // Implement free_array to free the array

    return result;
}
