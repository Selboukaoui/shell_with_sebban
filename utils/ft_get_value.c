/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:33:32 by asebban           #+#    #+#             */
/*   Updated: 2025/05/03 17:28:54 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/minishell.h"

static int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\v' || c == '\f' || c == '\r');
}

// static char *ft_strncpy(char *dest, const char *src, size_t n)
// {
//     size_t i = 0;

//     while (i < n && src[i])
//     {
//         dest[i] = src[i];
//         i++;
//     }
//     while (i < n)
//     {
//         dest[i] = '\0';
//         i++;
//     }
//     return dest;
// }

// static void int_to_str(int num, char *str)
// {
//     int i;
//     int j;
//     int is_negative;

//     j = 0;
//     i = 0;
//     is_negative = 0;
//     if (num == 0)
//     {
//         str[i++] = '0';
//         str[i] = '\0';
//         return;
//     }
//     if (num < 0)
//     {
//         is_negative = 1;
//         num = -num;
//     }
//     while (num != 0)
//     {
//         str[i++] = (num % 10) + '0';
//         num /= 10;
//     }

//     if (is_negative)
//         str[i++] = '-';
//     str[i] = '\0';

//     while(j < i / 2)
//     {
//         char temp;
//         temp = str[j];
//         str[j] = str[i - j - 1];
//         str[i - j - 1] = temp;
//         j++;
//     }
// }

// char *get_env_value(t_environ_list *env_list, char *key)
// {
//     t_environ_node *curr = env_list->head;

//     while (curr)
//     {
//         if (ft_strcmp(curr->key, key) == 0)
//             return curr->value;
//         curr = curr->next;
//     }
//     return NULL;
// }


// // char *replace_vars(char *input, t_shell *shell)
// // {
// //     size_t  i;
// //     size_t  j;
// //     size_t  len;
// //     char    *output;
// //     int     k;
// //     int     in_single_quote;
// //     int     in_double_quote;

// //     in_double_quote = 0;
// //     in_single_quote = 0;
// //     i = 0;
// //     j = 0;
// //     k = 0;
// //     len = ft_strlen(input);
// //     output = ft_malloc(PATH_MAX + 1, 1); // replace with ft_malloc
// //     if (!output)
// //         return NULL;

// //     while (i < len)
// //     {
// //         if (input[i] == '\'' && !in_double_quote)
// //         {
// //             in_single_quote = !in_single_quote;
// //             output[j++] = input[i++];
// //         }
// //         else if (input[i] == '\"' && !in_single_quote)
// //         {
// //             in_double_quote = !in_double_quote;
// //             output[j++] = input[i++];
// //         }
// //         else if (input[i] == '$' && input[i + 1] == '?' && !in_single_quote)
// //         {
// //             int status = exit_status(0, 0);
// //             char status_str[12];
// //             int_to_str(status, status_str);
// //             while (status_str[k])
// //                 output[j++] = status_str[k++];
// //             i += 2;
// //         }
// //         // Handle $VARNAME
// //         else if (input[i] == '$' && ft_isalpha(input[i + 1]) && !in_single_quote)
// //         {
// //             int var_start = i + 1;
// //             int var_len = 0;

// //             while (ft_isalnum(input[var_start + var_len]) || input[var_start + var_len] == '_')
// //                 var_len++;

// //             char var_name[256];
// //             ft_strncpy(var_name, &input[var_start], var_len);
// //             var_name[var_len] = '\0';

// //             char *env_value = get_env_value(shell->env, var_name);
// //             if (env_value)
// //             {
// //                 k = 0;
// //                 while (env_value[k])
// //                 output[j++] = env_value[k++];
// //             }
// //             else
// //             {
// //                 size_t tmp = i + var_len + 1;
// //                 int only_spaces_after = 1;
            
// //                 while (input[tmp])
// //                 {
// //                     if (!ft_isspace(input[tmp]))
// //                     {
// //                         only_spaces_after = 0;
// //                         break;
// //                     }
// //                     tmp++;
// //                 }
            
// //                 if (only_spaces_after)
// //                     output[j++] = '\n';
// //                 // else: do nothing (just remove the missing $VAR silently)
// //             }
            
// //             i += var_len + 1;
// //         }
// //         // If $ inside single quotes, treat as normal char
// //         else
// //         {
// //             output[j++] = input[i++];
// //         }
// //     }

// //     output[j] = '\0';
// //     return output;
// // }

// char *replace_vars(char *input, t_shell *shell)
// {
//     size_t  i = 0, j = 0, len = ft_strlen(input);
//     char    *output;
//     int     in_single_quote = 0, in_double_quote = 0;
//     int     k;

//     output = ft_malloc(PATH_MAX + 1, 1);
//     if (!output)
//         return NULL;

//     while (i < len)
//     {
//         /* handle quotes as before */
//         if (input[i] == '\'' && !in_double_quote)
//             in_single_quote = !in_single_quote, output[j++] = input[i++];
//         else if (input[i] == '\"' && !in_single_quote)
//             in_double_quote = !in_double_quote, output[j++] = input[i++];
//         /* variable or delimiter context */
//         else if (input[i] == '$' && !in_single_quote)
//         {
//             /* LOOKBACK: skip spaces before '$' */
//             int look = i - 1;
//             while (look >= 0 && ft_isspace(input[look]))
//                 look--;
//             /* if two '<' right before, it’s a heredoc delimiter => copy literally */
//             if (look >= 1 && input[look] == '<' && input[look-1] == '<')
//             {
//                 /* copy '$' */
//                 output[j++] = input[i++];
//                 /* copy var name literally */
//                 while (ft_isalnum(input[i]) || input[i] == '_')
//                     output[j++] = input[i++];
//             }
//             /* $? expansion */
//             else if (input[i+1] == '?')
//             {
//                 int status = exit_status(0, 0);
//                 char status_str[12];
//                 int_to_str(status, status_str);
//                 k = 0;
//                 while (status_str[k])
//                     output[j++] = status_str[k++];
//                 i += 2;
//             }
//             /* normal $VARNAME expansion */
//             else if (ft_isalpha(input[i+1]))
//             {
//                 int var_start = i + 1, var_len = 0;
//                 while (ft_isalnum(input[var_start + var_len]) ||
//                        input[var_start + var_len] == '_')
//                     var_len++;
//                 char var_name[256];
//                 ft_strncpy(var_name, &input[var_start], var_len);
//                 var_name[var_len] = '\0';
//                 char *env_value = get_env_value(shell->env, var_name);
//                 if (env_value)
//                 {
//                     k = 0;
//                     while (env_value[k])
//                         output[j++] = env_value[k++];
//                 }
//                 else
//                 {
//                     /* if missing and only spaces follow, emit newline */
//                     size_t tmp = i + var_len + 1;
//                     int only_spaces = 1;
//                     while (input[tmp])
//                         if (!ft_isspace(input[tmp++]))
//                             only_spaces = 0;
//                     if (only_spaces)
//                         output[j++] = '\n';
//                 }
//                 i += var_len + 1;
//             }
//             else
//             {
//                 /* just a lone '$' or non-alpha after it */
//                 output[j++] = input[i++];
//             }
//         }
//         /* all other chars */
//         else
//         {
//             output[j++] = input[i++];
//         }
//     }

//     output[j] = '\0';
//     return output;
// }

#include "../includes/minishell.h"

/*–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/
/* helpers (unchanged)                                              */
// static int ft_isspace(int c)
// {
//     return (c == ' ' || c == '\t' || c == '\n' ||
//             c == '\v' || c == '\f' || c == '\r');
// }

char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i = 0;

    /* copy up to n chars or until src ends */
    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }

    /* pad the rest with '\0' */
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }

    return dest;
}


void int_to_str(int num, char *str)
{
    int i = 0, j = 0, is_negative = 0;
    if (num == 0)
    {
        str[i++] = '0';
        str[i]   = '\0';
        return;
    }
    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }
    while (num)
    {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    if (is_negative)
        str[i++] = '-';
    str[i] = '\0';

    /* reverse */
    while (j < i / 2)
    {
        char tmp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = tmp;
        j++;
    }
}

char *get_env_value(t_environ_list *env_list, char *key)
{
    t_environ_node *cur = env_list->head;
    while (cur)
    {
        if (ft_strcmp(cur->key, key) == 0)
            return cur->value;
        cur = cur->next;
    }
    return NULL;
}
/*–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––*/

// char *replace_vars(char *input, t_shell *shell)
// {
//     size_t  i = 0, j = 0, len = ft_strlen(input);
//     char    *output;
//     int     in_single_quote   = 0;
//     int     in_double_quote   = 0;
//     int     seen_export       = 0;
//     int     in_export_assign  = 0;
//     int     k;

//     /* detect leading “export …” */
//     {
//         size_t p = 0;
//         while (p < len && ft_isspace(input[p])) p++;
//         if (p + 6 <= len
//             && ft_strncmp(&input[p], "export", 6) == 0
//             && (p + 6 == len || ft_isspace(input[p + 6])))
//         {
//             seen_export = 1;
//         }
//     }

//     output = ft_malloc(PATH_MAX + 1, 1);
//     if (!output)
//         return NULL;

//     while (i < len)
//     {
//         /* start of RHS of “export VAR=” */
//         if (seen_export && !in_export_assign
//             && !in_single_quote && !in_double_quote
//             && input[i] == '=')
//         {
//             in_export_assign = 1;
//             output[j++]     = input[i++];
//             continue;
//         }

//         /* leave export‐assignment context on unquoted space or pipe */
//         if (in_export_assign && !in_single_quote && !in_double_quote
//             && (ft_isspace(input[i]) || input[i] == '|'))
//         {
//             in_export_assign = 0;
//             output[j++]      = input[i++];
//             continue;
//         }

//         /* normal quote toggling */
//         if (input[i] == '\'' && !in_double_quote)
//             in_single_quote = !in_single_quote, output[j++] = input[i++];
//         else if (input[i] == '"' && !in_single_quote)
//             in_double_quote = !in_double_quote, output[j++] = input[i++];

//         /* variable expansion */
//         else if (input[i] == '$' && !in_single_quote)
//         {
//             /* in export‐assignment? wrap in quotes */
//             if (seen_export && in_export_assign)
//             {
//                 output[j++] = '"';

//                 /* handle $? */
//                 if (input[i + 1] == '?')
//                 {
//                     int  status;
//                     char status_str[12];
//                     status = exit_status(0, 0);
//                     int_to_str(status, status_str);
//                     k = 0;
//                     while (status_str[k])
//                         output[j++] = status_str[k++];
//                     i += 2;
//                 }
//                 /* normal $VARNAME */
//                 else if (ft_isalpha(input[i + 1]))
//                 {
//                     int  var_start = i + 1;
//                     int  var_len   = 0;
//                     char var_name[256];

//                     while ((ft_isalnum(input[var_start + var_len]) ||
//                             input[var_start + var_len] == '_'))
//                         var_len++;
//                     ft_strncpy(var_name, &input[var_start], var_len);
//                     var_name[var_len] = '\0';

//                     /* lookup and copy */
//                     {
//                         char *val = get_env_value(shell->env, var_name);
//                         k = 0;
//                         if (val)
//                             while (val[k])
//                                 output[j++] = val[k++];
//                     }
//                     i += var_len + 1;
//                 }
//                 else
//                 {
//                     /* just a lone $ or non‐alpha */
//                     output[j++] = input[i++];
//                 }

//                 output[j++] = '"';
//             }
//             /* not in export context: do original expansion */
//             else
//             {
//                 /* heredoc delimiter protection: <<… */
//                 int look = i - 1;
//                 while (look >= 0 && ft_isspace(input[look]))
//                     look--;
//                 if (look >= 1
//                     && input[look] == '<'
//                     && input[look - 1] == '<')
//                 {
//                     output[j++] = input[i++];
//                     while (ft_isalnum(input[i]) || input[i] == '_')
//                         output[j++] = input[i++];
//                 }
//                 /* $? expansion */
//                 else if (input[i + 1] == '?')
//                 {
//                     int  status;
//                     char status_str[12];
//                     status = exit_status(0, 0);
//                     int_to_str(status, status_str);
//                     k = 0;
//                     while (status_str[k])
//                         output[j++] = status_str[k++];
//                     i += 2;
//                 }
//                 /* normal $VARNAME */
//                 else if (ft_isalpha(input[i + 1]))
//                 {
//                     int var_start = i + 1;
//                     int var_len   = 0;
//                     char var_name[256];

//                     while ((ft_isalnum(input[var_start + var_len]) ||
//                             input[var_start + var_len] == '_'))
//                         var_len++;
//                     ft_strncpy(var_name, &input[var_start], var_len);
//                     var_name[var_len] = '\0';

//                     {
//                         char *val = get_env_value(shell->env, var_name);
//                         k = 0;
//                         if (val)
//                             while (val[k])
//                                 output[j++] = val[k++];
//                         else
//                         {
//                             /* if missing and only spaces follow → newline */
//                             size_t tmp = i + var_len + 1;
//                             int    only_spaces = 1;
//                             while (input[tmp])
//                                 if (!ft_isspace(input[tmp++]))
//                                     only_spaces = 0;
//                             if (only_spaces)
//                                 output[j++] = '\n';
//                         }
//                     }
//                     i += var_len + 1;
//                 }
//                 else
//                 {
//                     /* lone $ or non‐alpha */
//                     output[j++] = input[i++];
//                 }
//             }
//         }
//         else
//         {
//             /* everything else */
//             output[j++] = input[i++];
//         }
//     }

//     output[j] = '\0';
//     return output;
// }


// char *replace_vars(char *input, t_shell *shell)
// {
//     size_t  i = 0, j = 0, len = ft_strlen(input);
//     char    *output;
//     int     in_single_quote   = 0;
//     int     in_double_quote   = 0;
//     int     seen_export       = 0;
//     int     in_export_assign  = 0;
//     int     k;

//     /* detect leading “export …” */
//     {
//         size_t p = 0;
//         while (p < len && ft_isspace(input[p])) p++;
//         if (p + 6 <= len
//             && ft_strncmp(&input[p], "export", 6) == 0
//             && (p + 6 == len || ft_isspace(input[p + 6])))
//         {
//             seen_export = 1;
//         }
//     }

//     output = ft_malloc(PATH_MAX + 1, 1);
//     if (!output)
//         return NULL;

//     while (i < len)
//     {
//         /* start of RHS of “export VAR=” */
//         if (seen_export && !in_export_assign
//             && !in_single_quote && !in_double_quote
//             && input[i] == '=')
//         {
//             in_export_assign = 1;
//             output[j++]     = input[i++];
//             continue;
//         }

//         /* leave export‐assignment on unquoted space or pipe */
//         if (in_export_assign && !in_single_quote && !in_double_quote
//             && (ft_isspace(input[i]) || input[i] == '|'))
//         {
//             in_export_assign = 0;
//             output[j++]      = input[i++];
//             continue;
//         }

//         /* normal quote toggling */
//         if (input[i] == '\'' && !in_double_quote)
//             in_single_quote = !in_single_quote, output[j++] = input[i++];
//         else if (input[i] == '"' && !in_single_quote)
//             in_double_quote = !in_double_quote, output[j++] = input[i++];

//         /* potential variable expansion */
//         else if (input[i] == '$' && !in_single_quote)
//         {
//             /* --- NEW: literal `$` if next char is neither `?` nor letter --- */
//             if (input[i + 1] != '?' && !ft_isalpha(input[i + 1]))
//             {
//                 output[j++] = '$';
//                 i++;
//                 continue;
//             }

//             /* in export‐assignment? wrap expansions in quotes */
//             if (seen_export && in_export_assign)
//                 output[j++] = '"';

//             /* handle `$?` */
//             if (input[i + 1] == '?')
//             {
//                 int  status = exit_status(0, 0);
//                 char status_str[12];
//                 int_to_str(status, status_str);
//                 k = 0;
//                 while (status_str[k])
//                     output[j++] = status_str[k++];
//                 i += 2;
//             }
//             /* normal `$VARNAME` */
//             else
//             {
//                 int  var_start = i + 1;
//                 int  var_len   = 0;
//                 char var_name[256];

//                 while (ft_isalnum(input[var_start + var_len]) ||
//                        input[var_start + var_len] == '_')
//                     var_len++;

//                 ft_strncpy(var_name, &input[var_start], var_len);
//                 var_name[var_len] = '\0';

//                 char *val = get_env_value(shell->env, var_name);
//                 if (val)
//                 {
//                     k = 0;
//                     while (val[k])
//                         output[j++] = val[k++];
//                 }
//                 else
//                 {
//                     /* missing var: newline if only spaces follow */
//                     size_t tmp = var_start + var_len;
//                     int    only_spaces = 1;
//                     while (input[tmp])
//                         if (!ft_isspace(input[tmp++]))
//                             only_spaces = 0;
//                     if (only_spaces)
//                         output[j++] = '\n';
//                 }
//                 i += var_len + 1;
//             }

//             if (seen_export && in_export_assign)
//                 output[j++] = '"';
//         }
//         /* everything else */
//         else
//         {
//             output[j++] = input[i++];
//         }
//     }

//     output[j] = '\0';
//     return output;
// }
// char *replace_vars(char *input, t_shell *shell)
// {
//     size_t  i = 0, j = 0, len = ft_strlen(input);
//     char    *output;
//     int     in_single_quote   = 0;
//     int     in_double_quote   = 0;
//     int     seen_export       = 0;
//     int     in_export_assign  = 0;
//     int     k;

//     /* detect leading “export …” */
//     {
//         size_t p = 0;
//         while (p < len && ft_isspace(input[p]))
//             p++;
//         if (p + 6 <= len
//             && ft_strncmp(&input[p], "export", 6) == 0
//             && (p + 6 == len || ft_isspace(input[p + 6])))
//         {
//             seen_export = 1;
//         }
//     }

//     output = ft_malloc(PATH_MAX + 1, 1);
//     if (!output)
//         return NULL;

//     while (i < len)
//     {
//         /* start of RHS of “export VAR=” */
//         if (seen_export && !in_export_assign
//             && !in_single_quote && !in_double_quote
//             && input[i] == '=')
//         {
//             in_export_assign = 1;
//             output[j++]     = input[i++];
//             continue;
//         }

//         /* leave export‐assignment on unquoted space or pipe */
//         if (in_export_assign && !in_single_quote && !in_double_quote
//             && (ft_isspace(input[i]) || input[i] == '|'))
//         {
//             in_export_assign = 0;
//             output[j++]      = input[i++];
//             continue;
//         }

//         /* normal quote toggling */
//         if (input[i] == '\'' && !in_double_quote)
//             in_single_quote = !in_single_quote, output[j++] = input[i++];
//         else if (input[i] == '"' && !in_single_quote)
//             in_double_quote = !in_double_quote, output[j++] = input[i++];

//         /* potential variable expansion */
//         else if (input[i] == '$' && !in_single_quote)
//         {
//             /* handle literal end-of-input */
//             if (input[i + 1] == '\0')
//             {
//                 output[j++] = '$';
//                 i++;
//             }
//             /* handle `$?` */
//             else if (input[i + 1] == '?')
//             {
//                 if (seen_export && in_export_assign)
//                     output[j++] = '"';

//                 int  status = exit_status(0, 0);
//                 char status_str[12];
//                 int_to_str(status, status_str);
//                 k = 0;
//                 while (status_str[k])
//                     output[j++] = status_str[k++];
//                 i += 2;

//                 if (seen_export && in_export_assign)
//                     output[j++] = '"';
//             }
//             /* handle positional `$<digit>` (skip) */
//             else if (ft_isdigit(input[i + 1]))
//             {
//                 /* skip `$N` */
//                 i += 2;
//             }
//             /* normal `$VARNAME` */
//             else if (ft_isalpha(input[i + 1]) || input[i + 1] == '_')
//             {
//                 if (seen_export && in_export_assign)
//                     output[j++] = '"';

//                 int  var_start = i + 1;
//                 int  var_len   = 0;
//                 char var_name[256];

//                 while (ft_isalnum(input[var_start + var_len]) || input[var_start + var_len] == '_')
//                     var_len++;

//                 ft_strncpy(var_name, &input[var_start], var_len);
//                 var_name[var_len] = '\0';

//                 char *val = get_env_value(shell->env, var_name);
//                 if (val)
//                 {
//                     k = 0;
//                     while (val[k])
//                         output[j++] = val[k++];
//                 }
//                 else
//                 {
//                     /* missing var: newline if only spaces follow */
//                     size_t tmp = var_start + var_len;
//                     int    only_spaces = 1;
//                     while (input[tmp])
//                         if (!ft_isspace(input[tmp++]))
//                             only_spaces = 0;
//                     if (only_spaces)
//                         output[j++] = '\n';
//                 }
//                 i += var_len + 1;

//                 if (seen_export && in_export_assign)
//                     output[j++] = '"';
//             }
//             /* invalid or literal `$` */
//             else
//             {
    //                 output[j++] = '$';
    //                 i++;
    //             }
    //             continue;
    //         }
    //         /* everything else */
    //         else
    //         {
        //             output[j++] = input[i++];
        //         }
        //     }
        
        //     output[j] = '\0';
        //     return output;
        // }
        
        
        static int already_quoted(const char *s)
        {
            size_t n = strlen(s);
            if (n < 2) return 0;
            if (s[0] == '\'' && s[n-1] == '\'') return 1;
            if (s[0] == '"'  && s[n-1] == '"')  return 1;
            return 0;
        }
        
        /**
         * Wrap exactly one word in double-quotes if it contains:
         *  - whitespace, or
         *  - a single-quote (')
         *
         * E.g.  -la       → strdup("-la")
         *       foo bar   → "\"foo bar\""
         *       '-la'     → "\"'-la'\""
         */
        static void free_split(char **arr)
{
    if (!arr)
        return;
    for (size_t i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}

        static char *auto_quote_word(const char *word)
        {
            int need = 0;
            for (const char *p = word; *p; p++)
                if (*p == ' ' || *p == '\t' || *p == '\'')
                    { need = 1; break; }
        
            if (!need)
                return strdup(word);
        
            size_t len = strlen(word);
            char *out = malloc(len + 3);  // two " + NUL
            if (!out) return NULL;
            sprintf(out, "\"%s\"", word);
            return out;
        }
        
char *replace_vars(char *input, t_shell *shell)
{
    size_t i = 0, j = 0, L = ft_strlen(input);
    char  *output = ft_malloc(PATH_MAX + 1, 1);
    int    in_sq = 0, in_dq = 0;
    int    seen_export = 0, in_export_assign = 0;
    int    in_hdoc = 0, hdoc_word_started = 0;

    /* 1) detect leading “export” */
    {
        size_t p = 0;
        while (p < L && ft_isspace(input[p])) p++;
        if (p + 6 <= L && ft_strncmp(&input[p], "export", 6) == 0
            && (p + 6 == L || ft_isspace(input[p+6])))
            seen_export = 1;
    }

    while (i < L)
    {
        /* detect heredoc operator << */
        if (!in_sq && !in_dq && input[i] == '<' && input[i+1] == '<')
        {
            in_hdoc = 1;
            hdoc_word_started = 0;
            output[j++] = input[i++];  // '<'
            output[j++] = input[i++];  // '<'
            continue;
        }

        /* skip expansion for the heredoc delimiter word */
        if (in_hdoc)
        {
            char c = input[i];
            /* before starting word, copy whitespace */
            if (!hdoc_word_started)
            {
                if (ft_isspace(c))
                {
                    output[j++] = c;
                    i++;
                    continue;
                }
                hdoc_word_started = 1;
            }
            /* once word started, copy until whitespace then end skipping */
            if (hdoc_word_started)
            {
                if (ft_isspace(c) || c == '|')
                {
                    output[j++] = c;
                    i++;
                    in_hdoc = 0;
                    continue;
                }
                /* copy any character (including '$') literally */
                output[j++] = c;
                i++;
                continue;
            }
        }

        /* 2) track export-assignment */
        if (seen_export && !in_export_assign && !in_sq && !in_dq && input[i] == '=')
            { in_export_assign = 1; output[j++] = input[i++]; continue; }
        if (in_export_assign && !in_sq && !in_dq
            && (ft_isspace(input[i]) || input[i] == '|'))
            { in_export_assign = 0; output[j++] = input[i++]; continue; }

        /* 3) quote toggles */
        if (input[i] == '\'' && !in_dq)
            { in_sq = !in_sq; output[j++] = input[i++]; continue; }
        if (input[i] == '"' && !in_sq)
            { in_dq = !in_dq; output[j++] = input[i++]; continue; }

        /* 4) variable expansion */
        if (input[i] == '$' && !in_sq)
        {
            if (input[i+1] == '\0')        { output[j++] = '$'; i++; continue; }
            if (input[i+1] == '?')
            {
                int st; char buf[16];
                st = exit_status(0,0);
                int_to_str(st, buf);
                for (int k = 0; buf[k]; k++) output[j++] = buf[k];
                i += 2;
                continue;
            }
            if (ft_isdigit(input[i+1]))    { i += 2; continue; }
            if (ft_isalpha(input[i+1]) || input[i+1] == '_')
            {
                size_t vs = i+1, vl = 0;
                while (ft_isalnum(input[vs+vl]) || input[vs+vl] == '_') vl++;
                char name[vl+1];
                ft_strncpy(name, input + vs, vl);
                name[vl] = '\0';
                char *val = get_env_value(shell->env, name);
                if (val)
                {
                    if (seen_export && in_export_assign)
                    {
                        if (!already_quoted(val)) output[j++] = '"';
                        for (int k=0; val[k]; k++) output[j++] = val[k];
                        if (!already_quoted(val)) output[j++] = '"';
                    }
                    else if (!in_dq)
                    {
                        char **words = ft_split(val, ' ');
                        for (int w = 0; words[w]; w++)
                        {
                            char *q = auto_quote_word(words[w]);
                            for (int k=0; q[k]; k++) output[j++] = q[k];
                            free(q);
                            if (words[w+1]) output[j++] = ' ';
                        }
                        free_split(words);
                    }
                    else
                    {
                        for (int k = 0; val[k]; k++) output[j++] = val[k];
                    }
                }
                i = vs + vl;
                continue;
            }
        }

        /* 5) any other char */
        output[j++] = input[i++];
    }

    output[j] = '\0';
    return output;
}

        // char *replace_vars(char *input, t_shell *shell)
        // {
        //     size_t i = 0, j = 0, L = ft_strlen(input);
        //     char  *output = ft_malloc(PATH_MAX + 1, 1);
        //     int    in_sq = 0, in_dq = 0;
        //     int    seen_export = 0, in_export_assign = 0;
        
        //     /* 1) detect leading “export” */
        //     {
        //         size_t p = 0;
        //         while (p < L && ft_isspace(input[p])) p++;
        //         if (p+6 <= L
        //             && ft_strncmp(&input[p], "export", 6) == 0
        //             && (p+6 == L || ft_isspace(input[p+6])))
        //         {
        //             seen_export = 1;
        //         }
        //     }
        
        //     while (i < L)
        //     {
        //         /* 2) track export-assignment */
        //         if (seen_export && !in_export_assign && !in_sq && !in_dq && input[i] == '=')
        //         {
        //             in_export_assign = 1;
        //             output[j++] = input[i++];
        //             continue;
        //         }
        //         if (in_export_assign && !in_sq && !in_dq
        //             && (ft_isspace(input[i]) || input[i] == '|'))
        //         {
        //             in_export_assign = 0;
        //             output[j++] = input[i++];
        //             continue;
        //         }
        
        //         /* 3) quote toggles */
        //         if (input[i] == '\'' && !in_dq)
        //             { in_sq = !in_sq; output[j++] = input[i++]; continue; }
        //         if (input[i] == '"' && !in_sq)
        //             { in_dq = !in_dq; output[j++] = input[i++]; continue; }
        
        //         /* 4) variable expansion */
        //         if (input[i] == '$' && !in_sq)
        //         {
        //             /* end, $?, $digit: same as before */
        //             if (input[i+1] == '\0')        { output[j++] = '$'; i++; continue; }
        //             if (input[i+1] == '?')         {
        //                 int st = exit_status(0,0);
        //                 char buf[16]; int_to_str(st, buf);
        //                 for (int k=0; buf[k]; k++) output[j++] = buf[k];
        //                 i += 2; continue;
        //             }
        //             if (ft_isdigit(input[i+1]))    { i += 2; continue; }
        
        //             if (ft_isalpha(input[i+1]) || input[i+1]=='_')
        //             {
        //                 /* extract var name */
        //                 size_t vs = i+1, vl = 0;
        //                 while (ft_isalnum(input[vs+vl]) || input[vs+vl]=='_') vl++;
        //                 char name[vl+1];
        //                 ft_strncpy(name, input+vs, vl);
        //                 name[vl] = '\0';
        
        //                 char *val = get_env_value(shell->env, name);
        //                 if (val)
        //                 {
        //                     if (seen_export && in_export_assign)
        //                     {
        //                         /* wrap entire value once */
        //                         if (!already_quoted(val))
        //                             output[j++] = '"';
        //                         for (int k=0; val[k]; k++)
        //                             output[j++] = val[k];
        //                         if (!already_quoted(val))
        //                             output[j++] = '"';
        //                     }
        //                     else if (!in_dq)
        //                     {
        //                         /* command context: per-word quoting */
        //                         char **words = ft_split(val, ' ');
        //                         for (int w = 0; words[w]; w++)
        //                         {
        //                             char *q = auto_quote_word(words[w]);
        //                             for (int k=0; q[k]; k++)
        //                                 output[j++] = q[k];
        //                             free(q);
        //                             if (words[w+1])
        //                                 output[j++] = ' ';
        //                         }
        //                         free_split(words);
        //                     }
        //                     else
        //                     {
        //                         /* inside double-quotes: raw */
        //                         for (int k=0; val[k]; k++)
        //                             output[j++] = val[k];
        //                     }
        //                 }
        
        //                 i = vs + vl;
        //                 continue;
        //             }
        //         }
        
        //         /* 5) any other char */
        //         output[j++] = input[i++];
        //     }
        
        //     output[j] = '\0';
        //     return output;
        // }