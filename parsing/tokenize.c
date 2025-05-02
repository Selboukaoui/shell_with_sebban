/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:35:04 by asebban           #+#    #+#             */
/*   Updated: 2025/05/02 18:59:35 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// t_token which_type(char *s, char *rl_copy)
// {
//     static i = 0;
//     int j = 0;
//     int inquote = 0;
//     int before = 0;
//     int after = 0;
//     int len = ft_strlen(rl_copy + i);
//     while(rl_copy[i])
//     {
        
//         //check if it befor quote and after quote(in case if it operator like "|" >> ...)
//     }
//     if (!s)
//         return (EMPTY);
    
//     if (ft_strcmp(s, "|") == 0)
//         return (PIPE);
//     else if (ft_strcmp(s, "<") == 0)
//         return (REDERECT_IN);
//     else if (ft_strcmp(s, ">") == 0)
//         return (REDERECT_OUT);
//     else if (ft_strcmp(s, "<<") == 0)
//         return (HEREDOC);
//     else if (ft_strcmp(s, ">>") == 0)
//         return (APPEND);
//     return (CMD);
// }
// char *iterate_rl_copy(char *rl_copy)
// {
//     static int i = 0;  // Static variable to maintain position
//     char *current_char;

//     // If the string is null or we have reached the end of the string, return NULL
//     if (!rl_copy || rl_copy[i] == '\0')
//         return NULL;

//     // Get the current character at the current position
//     current_char = &rl_copy[i];

//     // Move the static variable i forward to the next character
//     i++;

//     return current_char;
// }

// t_token which_type(char *s, char *rl_copy)
// {
//     static int i = 0;  // Static variable to maintain the position
//     int inquote = 0;   // Flag to check if we're inside quotes
//     char *current_char;

//     // If the string is NULL or empty, return EMPTY
//     if (!s || !rl_copy || rl_copy[i] == '\0')
//         return EMPTY;

//     // Iterate through rl_copy using the static variable i
//     while ((current_char = iterate_rl_copy(rl_copy)) != NULL)
//     {
//         // Check if we encounter a quote
//         if (*current_char == '\'' || *current_char == '"')
//         {
//             if (!inquote)
//                 inquote = 1;  // Enter quoted section
//             else
//                 inquote = 0;  // Exit quoted section
//         }

//         // Only check for operators when we're outside quotes
//         if (!inquote)
//         {
//             // Check if the token matches an operator like "|", "<", ">", "<<", ">>", etc.
//             if (ft_strcmp(s, "|") == 0)
//                 return PIPE;
//             else if (ft_strcmp(s, "<") == 0)
//                 return REDERECT_IN;
//             else if (ft_strcmp(s, ">") == 0)
//                 return REDERECT_OUT;
//             else if (ft_strcmp(s, "<<") == 0)
//                 return HEREDOC;
//             else if (ft_strcmp(s, ">>") == 0)
//                 return APPEND;
//         }
//     }

//     return CMD;  // If no operator matched, return CMD
// }

// Returns index of the first occurrence of s in rl (starting at rl[last_pos]), or -1.
 // or whatever path brings in ft_strnstr and ft_strlen

// static int find_token_pos(const char *rl, const char *s, int last_pos)
// {
//     size_t rem_len = ft_strlen(rl + last_pos);
//     char *p = ft_strnstr(rl + last_pos, s, rem_len);
//     if (!p)
//         return -1;
//     return (int)(p - rl);
// }

// // Are we inside an unclosed quote at position idx?
// // We toggle inquote only when we see the matching opening quote.
// static int  is_inside_quotes(const char *rl, int idx)
// {
//     int  inquote    = 0;
//     char quote_char = 0;

//     for (int j = 0; j < idx; j++)
//     {
//         if (rl[j] == '\'' || rl[j] == '"')
//         {
//             if (!inquote)
//             {
//                 inquote    = 1;
//                 quote_char = rl[j];
//             }
//             else if (quote_char == rl[j])
//             {
//                 inquote    = 0;
//                 quote_char = 0;
//             }
//         }
//     }
//     return inquote;
// }


// t_token which_type(char *s, int inside)
// {
//     if (!s)
//         return EMPTY;
//     if (inside)
//         return CMD;   // anything inside quotes is a “word,” never an operator

//     if (ft_strcmp(s, "|")  == 0) return PIPE;
//     if (ft_strcmp(s, "<")  == 0) return REDERECT_IN;
//     if (ft_strcmp(s, ">")  == 0) return REDERECT_OUT;
//     if (ft_strcmp(s, "<<") == 0) return HEREDOC;
//     if (ft_strcmp(s, ">>") == 0) return APPEND;
//     return CMD;
// }



static t_lexer_list *create_lexer_list(char **input_array)
{
    t_lexer_list    *head;
    t_lexer_list    *current;
    t_lexer_list    *prev;
    int             size;
    int             i;

    if (!input_array)
        return (NULL);
    
    // Calculate size of input array
    size = 0;
    while (input_array[size])
        size++;
    
    if (size == 0)
        return (NULL);
    
    // Allocate first node
    head = (t_lexer_list *)ft_malloc(sizeof(t_lexer_list), 1);
    if (!head)
        return (NULL);
    head->prev = NULL;
    prev = head;
    
    // Allocate remaining nodes
    i = 1;
    while (i < size)
    {
        current = (t_lexer_list *)ft_malloc(sizeof(t_lexer_list), 1);
        if (!current)
        {
            // Cleanup on allocation failure
            while (head)
            {
                current = head->next;
                // //free(head);
                head = current;
            }
            return (NULL);
        }
        current->prev = prev;
        prev->next = current;
        prev = current;
        i++;
    }
    if (size > 0)
        prev->next = NULL;
    
    return (head);
}

// static t_lexer_list  *fill_lexer_list(char **input_array, const char *rl_copy)
// {
//     t_lexer_list *head = create_lexer_list(input_array);
//     t_lexer_list *cur  = head;
//     int           last_pos = 0;
//     int           idx;

//     for (int i = 0; input_array[i] && cur; i++, cur = cur->next)
//     {
//         cur->str = ft_strdup(input_array[i]);
//         if (!cur->str) { /* cleanup… */ return NULL; }

//         // 1) find where this token starts in rl_copy
//         idx = find_token_pos(rl_copy, cur->str, last_pos);
//         if (idx < 0) idx = last_pos;  
//         // 2) update last_pos so next search begins _after_ this token
//         last_pos = idx + ft_strlen(cur->str);

//         // 3) check if that position sits inside quotes
//         int inside = is_inside_quotes(rl_copy, idx);

//         // 4) assign the correct type
//         cur->type = which_type(cur->str, inside);
//     }

//     return head;
// }


// // static t_lexer_list *fill_lexer_list(char **input_array, char *rl_copy)
// // {
// //     t_lexer_list    *head;
// //     t_lexer_list    *current;
// //     int             i;

// //     head = create_lexer_list(input_array);
// //     if (!head)
// //         return (NULL);
    
// //     current = head;
// //     i = 0;
// //     while (input_array[i] && current)
// //     {
// //         current->str = ft_strdup(input_array[i]);
// //         if (!current->str)
// //         {
// //             // Cleanup on allocation failure
// //             while (head)
// //             {
// //                 current = head->next;
// //                 // //free(head->str);
// //                 // //free(head);
// //                 head = current;
// //             }
// //             return (NULL);
// //         }
// //         current->type = which_type(current->str, rl_copy);
// //         current = current->next;
// //         i++;
// //     }
    
// //     return (head);
// // }

// t_lexer_list *tokenize(t_shell *shell)
// {
//     char            **input_array;
//     t_lexer_list    *lexer_list;

//     if (!shell || !shell->rl_copy)
//         return (NULL);
//     input_array = ft_newsplit(shell->rl_copy);
//     if (!input_array)
//         return (NULL);
    
//     lexer_list = fill_lexer_list(input_array, shell->rl_copy);
//     // free_str_arr(input_array);
//     return (lexer_list);
// }#include "../includes/minishell.h"

/*
 * Find the position of token s in rl, starting from last_pos.
 */
static int find_token_pos(const char *rl, const char *s, int last_pos)
{
    size_t rem_len = ft_strlen(rl + last_pos);
    char *p = ft_strnstr(rl + last_pos, s, rem_len);
    if (!p)
        return -1;
    return (int)(p - rl);
}

/*
 * Are we inside an unclosed quote at position idx in rl?
 */
static int is_inside_quotes(const char *rl, int idx)
{
    int  inquote    = 0;
    char quote_char = 0;

    for (int j = 0; j < idx; j++)
    {
        if (rl[j] == '\'' || rl[j] == '"')
        {
            if (!inquote)
            {
                inquote    = 1;
                quote_char = rl[j];
            }
            else if (quote_char == rl[j])
            {
                inquote    = 0;
                quote_char = 0;
            }
        }
    }
    return inquote;
}

/*
 * Classify a string token: operators (unless inside quotes) vs. CMD.
 */
t_token which_type(char *s, int inside)
{
    if (!s)
        return EMPTY;
    if (inside)
        return CMD;

    if (ft_strcmp(s, "|")  == 0) return PIPE;
    if (ft_strcmp(s, "<")  == 0) return REDERECT_IN;
    if (ft_strcmp(s, ">")  == 0) return REDERECT_OUT;
    if (ft_strcmp(s, "<<") == 0) return HEREDOC;
    if (ft_strcmp(s, ">>") == 0) return APPEND;
    return CMD;
}

/*
 * Build a doubly-linked list of lexer tokens.  If the original line
 * (raw_rl) begins—after whitespace—with `$` and a valid name, all
 * tokens here are forced to CMD.
 */
t_lexer_list *fill_lexer_list(char **input_array,
                              const char *raw_rl,
                              const char *rl_copy)
{
    t_lexer_list *head = create_lexer_list(input_array);
    t_lexer_list *cur  = head;
    int           last_pos = 0;
    int           idx;

    /* Detect leading “$VAR” in the unexpanded raw RL */
    int override_all = 0;
    {
        int i = 0;
        while (raw_rl[i] == ' ' || raw_rl[i] == '\t')
            i++;
        if (raw_rl[i] == '$'
         && (ft_isalpha(raw_rl[i+1]) || raw_rl[i+1] == '_'))
            override_all = 1;
    }

    for (int i = 0; input_array[i] && cur; i++, cur = cur->next)
    {
        cur->str = ft_strdup(input_array[i]);
        if (!cur->str)
        {
            // TODO: free list
            return NULL;
        }

        /* 1) locate token in expanded rl_copy */
        idx = find_token_pos(rl_copy, cur->str, last_pos);
        if (idx < 0) idx = last_pos;
        last_pos = idx + ft_strlen(cur->str);

        /* 2) assign type */
        if (override_all)
        {
            cur->type = CMD;
        }
        else
        {
            int inside = is_inside_quotes(rl_copy, idx);
            cur->type = which_type(cur->str, inside);
        }
    }

    return head;
}

/*
 * Top-level tokenize: split via ft_newsplit on rl_copy, then build list.
 */
t_lexer_list *tokenize(t_shell *shell)
{
    if (!shell || !shell->rl_copy )
        return NULL;

    /* split expanded line into raw tokens */
    char **tokens = ft_newsplit(shell->rl_copy);
    if (!tokens)
        return NULL;

    /* build lexer list, passing both raw and expanded lines */
    t_lexer_list *lex = fill_lexer_list(tokens,
                                        shell->rl_input,
                                        shell->rl_copy);

    /* cleanup token array */
    for (size_t i = 0; tokens[i]; i++)
        free(tokens[i]);
    free(tokens);

    return lex;
}
