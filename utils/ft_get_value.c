/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:33:32 by asebban           #+#    #+#             */
/*   Updated: 2025/04/21 21:47:29 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\v' || c == '\f' || c == '\r');
}


char *replace_vars(char *input)
{
    size_t i = 0, j = 0;
    size_t len = ft_strlen(input);
    char *output = malloc(PATH_MAX + 1); // Allocate for output string

    if (!output)
        return NULL;

    while (i < len)
    {
        // Skip spaces
        if (ft_isspace(input[i]))
        {
            output[j++] = input[i++];
            continue;
        }

        // Check for "$?" replacement
        if (input[i] == '$' && input[i + 1] == '?')
        {
            // Replace with the exit status
            int status = exit_status(0, 0); // Get exit status
            char status_str[10];
            int temp = status, k = 0;

            // Convert exit status to string
            if (status == 0)
            {
                status_str[k++] = '0';
            }
            else
            {
                while (temp > 0)
                {
                    status_str[k++] = (temp % 10) + '0';
                    temp /= 10;
                }

                // Reverse the string
                for (int l = 0; l < k / 2; l++)
                {
                    char tmp = status_str[l];
                    status_str[l] = status_str[k - l - 1];
                    status_str[k - l - 1] = tmp;
                }
            }
            status_str[k] = '\0';

            // Append exit status to the output
            for (int l = 0; status_str[l] != '\0'; l++)
                output[j++] = status_str[l];

            i += 2;  // Skip over "$?"
        }
        // Check for environment variables like "$PATH"
        else if (input[i] == '$' && input[i + 1] != '\0')
        {
            int var_start = i + 1;
            int var_len = 0;

            // Find the end of the variable (only alphanumeric or underscores)
            while (isalnum(input[i + 1 + var_len]) || input[i + 1 + var_len] == '_')
                var_len++;

            char var_name[256]; // Store the environment variable name
            for (int k = 0; k < var_len; k++)
                var_name[k] = input[var_start + k];
            var_name[var_len] = '\0';

            // Get the environment variable value using getenv
            char *env_value = getenv(var_name);
            if (env_value)
            {
                // Append the value of the environment variable to output
                for (int k = 0; env_value[k] != '\0'; k++)
                    output[j++] = env_value[k];
            }
            else
            {
                // If not found, just append "$VAR" as it is
                output[j++] = '$';
                for (int k = 0; var_name[k] != '\0'; k++)
                    output[j++] = var_name[k];
            }

            i += var_len + 1;  // Skip over "$VAR"
        }
        else
        {
            // Regular character, just append to the output
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';  // Null terminate the output
    return output;
}
