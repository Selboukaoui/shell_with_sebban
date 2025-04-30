/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:02:28 by asebban           #+#    #+#             */
/*   Updated: 2025/04/16 14:02:28 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

// #include <stdlib.h>
#include <string.h>

/*
 * handle_dollar_quotes:
 *   - For sequences of '$' followed by a quoted string:
 *       * If exactly 1 dollar and quoted string empty ($"" or $''), output single '$'.
 *       * If quoted string non-empty:
 *           - If dollar_count is even, keep all '$'.
 *           - If odd, drop one '$'.
 *         Then append the unquoted content.
 *   - Otherwise, copy '$'s literally.
 */
static char *handle_dollar_quotes(const char *input)
{
    size_t len = strlen(input);
    /* Allocate with some headroom for expansions */
    size_t cap = len * 2 + 1;
    char *result = malloc(cap);
    if (!result)
        return NULL;

    size_t i = 0, j = 0;
    while (i < len) {
        if (input[i] == '$') {
            /* count run of '$' */
            size_t dollar_count = 0;
            while (i + dollar_count < len && input[i + dollar_count] == '$')
                dollar_count++;

            size_t k = i + dollar_count;
            /* if next char is a quote, apply special here-doc rules */
            if (k < len && (input[k] == '\'' || input[k] == '"')) {
                char quote = input[k];
                size_t var_start = k + 1;
                size_t m = var_start;
                /* find matching closing quote */
                while (m < len && input[m] != quote)
                    m++;
                size_t var_len = m - var_start;

                /* skip closing quote if present */
                if (m < len && input[m] == quote)
                    m++;

                /* empty quoted string after single '$' => literal '$' */
                if (dollar_count == 1 && var_len == 0) {
                    result[j++] = '$';
                } else {
                    /* determine how many '$' to keep */
                    size_t keep = (dollar_count % 2 == 0)
                                    ? dollar_count
                                    : dollar_count - 1;
                    for (size_t d = 0; d < keep; d++)
                        result[j++] = '$';
                    /* append unquoted content */
                    memcpy(result + j, input + var_start, var_len);
                    j += var_len;
                }
                /* advance past processed section */
                i = m;
                continue;
            }
            /* no quote after => copy all '$' literally */
            for (size_t d = 0; d < dollar_count; d++)
                result[j++] = '$';
            i = k;
        } else {
            result[j++] = input[i++];
        }
    }
    result[j] = '\0';
    return result;
}


// static char *handle_dollar_quotes(const char *input)
// {
//     int len = ft_strlen(input);
//     char *result = malloc(len + 1);
//     if (!result)
//         return NULL;

//     int i = 0, j = 0;
//     while (i < len)
//     {
//         if (input[i] == '$')
//         {
//             int dollar_count = 0;
//             int k = i;
//             while (k < len && input[k] == '$')
//                 dollar_count++, k++;

//             /* if next char is a quote, apply special rules */
//             if (k < len && (input[k] == '\'' || input[k] == '"'))
//             {
//                 if (dollar_count == 1)
//                 {
//                     /* single $ → preserve it */
//                     result[j++] = '$';
//                 }
//                 else if (dollar_count % 2 == 0)
//                 {
//                     /* even → copy them all */
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//                 else
//                 {
//                     /* odd >1 → drop one, copy the rest */
//                     dollar_count--;
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//             }
//             else
//             {
//                 /* no quote after → copy every $ */
//                 while (dollar_count--)
//                     result[j++] = '$';
//             }
//             i = k;
//         }
//         else
//         {
//             result[j++] = input[i++];
//         }
//     }
//     result[j] = '\0';
//     return result;
// }


// static char *handle_dollar_quotes(const char *input)
// {
//     int len = ft_strlen(input);
//     char *result = malloc(len + 1); // Allocate enough (final string will be smaller or same)
//     if (!result)
//         return NULL;

//     int i = 0, j = 0;
//     while (i < len)
//     {
//         if (input[i] == '$')
//         {
//             int dollar_count = 0;
//             int k = i;

//             // Count how many dollars
//             while (k < len && input[k] == '$')
//             {
//                 dollar_count++;
//                 k++;
//             }

//             // Check if next character is a quote
//             if (k < len && (input[k] == '\'' || input[k] == '"'))
//             {
//                 if (dollar_count % 2 == 0)
//                 {
//                     // Even number of dollars, copy all
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//                 else
//                 {
//                     // Odd number, remove one dollar
//                     dollar_count--;
//                     while (dollar_count--)
//                         result[j++] = '$';
//                 }
//             }
//             else
//             {
//                 // No quote after, copy all dollars
//                 while (dollar_count--)
//                     result[j++] = '$';
//             }
//             i = k;
//         }
//         else
//         {
//             result[j++] = input[i++];
//         }
//     }

//     result[j] = '\0';
//     return result;
// }

int main(int ac, char **av, char **env)
{
    t_shell *shell;

    (void)ac;
    (void)av;
    shell = init_shell(env);
    signal_setup(PARENT);
    
    if (!shell)
        return (1);
    while (1)
    {
        shell->rl_input = readline("minishell-1.0$~ ");
        if (!shell->rl_input) // CTRL+D (EOF)
		{
			write(1, "exit\n", 5);
            ft_malloc(0, 0);
			break; // command not found in ctr + d handle it
		}
        if (*shell->rl_input)
            add_history(shell->rl_input);

        // handle history cmd here
        shell->rl_input = handle_dollar_quotes(shell->rl_input);
        shell->rl_copy = clean_rl_copy(shell->rl_input);
        // printf ("rl->copy : %s\n",shell->rl_copy);
        shell->rl_copy = replace_vars(shell->rl_input, shell);
        //check syntax like ">>>"
        // if (!ft_strcmp(shell->rl_input, "\\"))
        // {
        //     ft_putstr_fd("syntax error near unexpected token`(xx)'\n", 2);
        //     //clean
        // }

        // printf ("%s-->\n", shell->rl_copy);
        if (parser(shell) == false)
            continue ;

        // t_shell *tst = shell;
        // while (tst->lex_head)
        // {
        //     printf ("str ----> %s\n type -----> %u\n", tst->lex_head->str, tst->lex_head->type);
        //     tst->lex_head = tst->lex_head->next;
        // }

        executor(shell);
        // and clean for next loop
    }
    // clean
    return(0);// return exit_code
}