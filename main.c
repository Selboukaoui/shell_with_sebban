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
static char *
handle_dollar_quotes(const char *input)
{
    size_t  len     = strlen(input);
    size_t  cap     = len + 1;
    char   *out     = ft_malloc(cap, 1);
    size_t  oi      = 0;
    bool    in_q    = false;

    for (size_t i = 0; i < len; /* updated below */) {
        char c = input[i];

        /* Toggle quote state on unescaped '"' */
        if (c == '"' && (i == 0 || input[i-1] != '\\')) {
            /* append the '"' */
            if (oi + 1 >= cap) {
                cap = cap * 2;
                out = realloc(out, cap);
            }
            out[oi++] = '"';
            in_q = !in_q;
            i++;
            continue;
        }

        /* If we’re outside quotes, check for $…+" pattern */
        if (!in_q && c == '$') {
            /* count consecutive $ */
            size_t j = i;
            while (j < len && input[j] == '$') j++;
            /* if next is a quote, handle per parity rule */
            if (j < len && input[j] == '"') {
                size_t dollar_count = j - i;
                if (dollar_count % 2 == 1) {
                    /* odd -> drop one $ */
                    if (oi + dollar_count - 1 + 1 >= cap) {
                        cap = cap + len;  /* enough room */
                        out = realloc(out, cap);
                    }
                    /* write (count-1) $ */
                    for (size_t k = 0; k < dollar_count - 1; k++) {
                        out[oi++] = '$';
                    }
                    /* write the quote, enter quote mode */
                    out[oi++] = '"';
                    in_q = true;
                } else {
                    /* even -> leave as-is (all $ and the ") */
                    if (oi + dollar_count + 1 >= cap) {
                        cap = cap + len;
                        out = realloc(out, cap);
                    }
                    for (size_t k = 0; k < dollar_count; k++) {
                        out[oi++] = '$';
                    }
                    out[oi++] = '"';
                    in_q = true;
                }
                i = j + 1;
                continue;
            }
            /* otherwise not followed by a quote—treat literally */
        }

        /* Default: copy character */
        if (oi + 1 >= cap) {
            cap = cap * 2;
            out = realloc(out, cap);
        }
        out[oi++] = c;
        i++;
    }

    out[oi] = '\0';
    return out;
}
static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

int is_cmdline_empty(const char *cmdline)
{
    if (!cmdline)
        return 1;

    int i = 0;
    while (cmdline[i])
    {
        // skip whitespace
        if (ft_isspace((unsigned char)cmdline[i])) {
            i++;
            continue;
        }

        // skip empty double quotes
        if (cmdline[i] == '"' && cmdline[i + 1] == '"' ) {
            i += 2;
            continue;
        }

        // skip empty single quotes
        if (cmdline[i] == '\'' && cmdline[i + 1] == '\'') {
            i += 2;
            continue;
        }

        // if any other non-space, non-empty-quote char is found, not empty
        return 0;
    }

    return 1;  // only whitespace or empty quotes
}

// char *replace_var_equals_var(char *input, t_shell *shell)
// {
//     char *output = ft_malloc(PATH_MAX + 1, 1);
//     size_t i = 0, j = 0;

//     while (input[i])
//     {
//         if (input[i] == '$')
//         {
//             // Try to parse variable name
//             size_t name_start = i + 1;
//             size_t name_len = 0;
//             while (ft_isalnum(input[name_start + name_len]) || input[name_start + name_len] == '_')
//                 name_len++;

//             if (name_len == 0)
//             {
//                 output[j++] = input[i++];
//                 continue;
//             }

//             char name[PATH_MAX];
//             ft_strncpy(name, input + name_start, name_len);
//             name[name_len] = '\0';

//             char *val = get_env_value(shell->env, name);
//             if (!val)
//                 val = "";

//             i = name_start + name_len;

//             // Check if we are in pattern like $x=$y or $x=$y=$z
//             if (input[i] == '=')
//             {
//                 // Write val of $x
//                 for (size_t k = 0; val[k]; k++) output[j++] = val[k];
//                 output[j++] = '='; // Write '='
//                 i++; // Skip '='

//                 while (input[i] == '$')
//                 {
//                     i++; // skip $
//                     size_t rlen = 0;
//                     while (ft_isalnum(input[i + rlen]) || input[i + rlen] == '_')
//                         rlen++;

//                     if (rlen == 0) break;

//                     char rname[PATH_MAX];
//                     ft_strncpy(rname, input + i, rlen);
//                     rname[rlen] = '\0';

//                     char *rval = get_env_value(shell->env, rname);
//                     if (!rval)
//                         rval = "";

//                     for (size_t k = 0; rval[k]; k++) output[j++] = rval[k];
//                     i += rlen;

//                     if (input[i] == '=')
//                     {
//                         output[j++] = '=';
//                         i++;
//                     }
//                     else break;
//                 }

//                 continue;
//             }
//             else
//             {
//                 // It's not in $x=$y form → leave untouched
//                 output[j++] = '$';
//                 for (size_t k = 0; k < name_len; k++) output[j++] = name[k];
//                 continue;
//             }
//         }
//         else
//         {
//             output[j++] = input[i++];
//         }
//     }

//     output[j] = '\0';
//     return output;
// }
char *replace_var_equals_var(char *input, t_shell *shell)
{
    size_t L = ft_strlen(input);
    size_t idx = 0;

    // 1) Skip leading spaces
    while (idx < L && ft_isspace(input[idx]))
        idx++;

    // 2) Skip optional "export" keyword + following spaces
    if (idx + 6 <= L
        && ft_strncmp(input + idx, "export", 6) == 0
        && (idx + 6 == L || ft_isspace(input[idx + 6])))
    {
        idx += 6;
        while (idx < L && ft_isspace(input[idx]))
            idx++;
    }

    // 3) If the first non-space (after export) isn't '$', leave unchanged
    if (idx >= L || input[idx] != '$')
        return ft_strdup(input);

    // 4) Otherwise, perform your chain-assignment replacement:
    char *output = ft_malloc(PATH_MAX + 1, 1);
    size_t i = 0, j = 0;

    while (input[i])
    {
        if (input[i] == '$')
        {
            // parse variable name after '$'
            size_t name_start = i + 1, name_len = 0;
            while (ft_isalnum(input[name_start + name_len]) ||
                   input[name_start + name_len] == '_')
                name_len++;

            if (name_len == 0)
            {
                output[j++] = input[i++];
                continue;
            }

            // copy var name into buffer
            char name[PATH_MAX];
            ft_strncpy(name, input + name_start, name_len);
            name[name_len] = '\0';

            // lookup its value (empty if unset)
            char *val = get_env_value(shell->env, name);
            if (!val) val = "";

            // advance input pointer past "$name"
            i = name_start + name_len;

            // only expand if next char is '='
            if (input[i] == '=')
            {
                // write left-side value + '='
                for (size_t k = 0; val[k]; k++)
                    output[j++] = val[k];
                output[j++] = '=';
                i++;

                // then expand any further $VAR[=…] segments
                while (input[i] == '$')
                {
                    i++;  // skip '$'
                    size_t rlen = 0;
                    while (ft_isalnum(input[i + rlen]) ||
                           input[i + rlen] == '_')
                        rlen++;

                    if (rlen == 0) break;

                    char rname[PATH_MAX];
                    ft_strncpy(rname, input + i, rlen);
                    rname[rlen] = '\0';

                    char *rval = get_env_value(shell->env, rname);
                    if (!rval) rval = "";

                    for (size_t k = 0; rval[k]; k++)
                        output[j++] = rval[k];
                    i += rlen;

                    if (input[i] == '=')
                    {
                        output[j++] = '=';
                        i++;
                    }
                    else
                        break;
                }
                continue;
            }
            else
            {
                // not a "$x=" chain -> copy literally
                output[j++] = '$';
                for (size_t k = 0; k < name_len; k++)
                    output[j++] = name[k];
                continue;
            }
        }
        else
        {
            // any other char, copy as-is
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
    return output;
}

int main(int ac, char **av, char **env)
{
    t_shell *shell;

    (void)ac;
    (void)av;
    g_signals = 1;
    rl_catch_signals = 0;
    shell = init_shell(env);
    signal_setup(PARENT);
    
    if (!shell)
        return (1);
    while (1)
    {
        g_signals = -70;
        if (!isatty(1) || !isatty(0))
            return (0);
        shell->rl_input = readline("minishell-1.0$~ ");
        g_signals = 1;
        if (!shell->rl_input) // CTRL+D (EOF)
		{
			write(1, "exit\n", 5);
            ft_malloc(0, 0);
			exit(exit_status(0,0)); // command not found in ctr + d handle it
		}
        if (*shell->rl_input)
            add_history(shell->rl_input);

        // handle history cmd here
        // printf("the cmd in readline:%s\n", shell->rl_input);
        shell->rl_input = handle_dollar_quotes(shell->rl_input);
        // printf("handle_dolar ----> %s\n", shell->rl_input);

        shell->rl_input = replace_var_equals_var(shell->rl_input, shell);
        // printf ("return your new func : %s\n",shell->rl_input);
        shell->rl_copy = clean_rl_copy(shell->rl_input);
        // printf ("clean_: %s\n",shell->rl_copy);

        shell->rl_copy = replace_vars(shell->rl_input, shell);
        // printf ("replace: %s\n",shell->rl_copy);
        // printf("the cmd after change all vars ----> %s\n", shell->rl_copy);
        // if (is_blank_command(shell->rl_copy)) {
        //     // maybe skip, or print "command not found"
        //     continue;
        // }
        
        
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












// static char *handle_dollar_quotes(const char *input)
// {
//     int len = ft_strlen(input);
//     char *result = ft_malloc(len + 1);
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
//     char *result = ft_malloc(len + 1); // Allocate enough (final string will be smaller or same)
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


// int main(int ac, char **av, char **env)
// {
//     t_shell *shell;

//     (void)ac;
//     (void)av;
//     g_signal = 0;
//     rl_catch_signals = 0;
//     shell = init_shell(env);
//     signal_setup(PARENT);
    
//     if (!shell)
//         return (1);
//     while (1)
//     {
//         g_signal = -1;
//         shell->rl_input = readline("minishell-1.0$~ ");
//         g_signal = 0;
//         if (!shell->rl_input) // CTRL+D (EOF)
// 		{
// 			write(1, "exit\n", 5);
//             ft_malloc(0, 0);
// 			break; // command not found in ctr + d handle it
// 		}
//         if (*shell->rl_input)
//             add_history(shell->rl_input);

//         // handle history cmd here
//         shell->rl_input = handle_dollar_quotes(shell->rl_input);
//         // printf("str_handle_dolar ----> %s\n", shell->rl_input);
//         shell->rl_copy = clean_rl_copy(shell->rl_input);
//         // printf ("rl->copy : %s\n",shell->rl_copy);
//         shell->rl_copy = replace_vars(shell->rl_input, shell);

//         // printf("str ----> %s\n", shell->rl_copy);
//         //check syntax like ">>>"
//         // if (!ft_strcmp(shell->rl_input, "\\"))
//         // {
//         //     ft_putstr_fd("syntax error near unexpected token`(xx)'\n", 2);
//         //     //clean
//         // }

//         // printf ("%s-->\n", shell->rl_copy);
//         if (parser(shell) == false)
//             continue ;

//         // t_shell *tst = shell;
//         // while (tst->lex_head)
//         // {
//         //     printf ("str ----> %s\n type -----> %u\n", tst->lex_head->str, tst->lex_head->type);
//         //     tst->lex_head = tst->lex_head->next;
//         // }

//         executor(shell);
//         // and clean for next loop
//     }
//     // clean
//     return(0);// return exit_code
// }
