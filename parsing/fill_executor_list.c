/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_executor_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:37 by asebban           #+#    #+#             */
/*   Updated: 2025/05/01 10:54:56 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static bool open_outputfile(t_executor *current, t_lexer_list *lexer)
// {
//     if (current->append)
//         current->fd_out = open(lexer->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
//     else
//         current->fd_out = open(lexer->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (current->fd_out == -1)
//     {
//         ft_putstr_fd("minishell: ", STDERR_FILENO);
//         // perror(lexer->str);
//         // return (false);
//     }
//     return (true);
// }
static bool open_outputfile(t_executor *current, t_lexer_list *lexer)
{
    if (current->append)
        current->fd_out = open(lexer->str,
            O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        current->fd_out = open(lexer->str,
            O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (current->fd_out == -1)
    {
        perror(lexer->str);
        return false;
    }
    return true;
}


static int process_out_append(t_executor *current, t_lexer_list *lexer)
{
    // if (!lexer->next || lexer->next->type != EMPTY)
    // {
    //     // ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
    //     return (FAILED);
    // }
    
    current->append = (lexer->type == APPEND);
    current->REDERECT_OUT = (lexer->type == REDERECT_OUT);
    if (!open_outputfile(current, lexer->next))
        return (FAILED);
    
    return (OK);
}

// static int create_heredoc(char *delimiter)
// {
//     int     fd[2];
//     char    *line;

//     if (pipe(fd) == -1)
//         return (-1);
    
//     while (1)
//     {
//         line = readline("> ");
//         if (!line || ft_strcmp(line, delimiter) == 0)
//         {
//             // //free(line);
//             break;
//         }
//         write(fd[1], line, ft_strlen(line));
//         write(fd[1], "\n", 1);
//         // //free(line);
//     }
//     close(fd[1]);
//     return (fd[0]);
// }

int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}


// static int is_delimiter_quoted_in_cmd(const char *cmdline, const char *delimiter, int heredoc_index)
// {
//     int i = 0;
//     int count = 0;
//     int in_single = 0, in_double = 0;

//     while (cmdline[i])
//     {
//         if (cmdline[i] == '\'' && !in_double)
//         {
//             in_single = !in_single;
//             i++;
//             continue;
//         }
//         if (cmdline[i] == '"' && !in_single)
//         {
//             in_double = !in_double;
//             i++;
//             continue;
//         }

//         // Check for `<<`
//         if (!in_single && !in_double && cmdline[i] == '<' && cmdline[i + 1] == '<')
//         {
//             i += 2;
//             while (cmdline[i] && ft_isspace(cmdline[i]))
//                 i++;

//             if (count == heredoc_index)
//             {
//                 if (cmdline[i] == '\'' || cmdline[i] == '"')
//                     return 1; // Delimiter was quoted
//                 return 0; // Not quoted
//             }

//             count++;
//         }
//         else
//             i++;
//     }

//     return 0; // Not found or not quoted
// }

static int is_delimiter_quoted_in_cmd(const char *cmdline, const char *delimiter, int heredoc_index)
{
    int i = 0;
    int count = 0;
    int in_single = 0, in_double = 0;

    (void)delimiter;
    while (cmdline[i])
    {
        if (cmdline[i] == '\'' && !in_double)
        {
            in_single = !in_single;
            i++;
            continue;
        }
        if (cmdline[i] == '"' && !in_single)
        {
            in_double = !in_double;
            i++;
            continue;
        }

        if (!in_single && !in_double && cmdline[i] == '<' && cmdline[i + 1] == '<')
        {
            i += 2;
            while (cmdline[i] && ft_isspace(cmdline[i]))
                i++;

            // Extract delimiter token from the command line
            int start = i;
            while (cmdline[i] && !ft_isspace(cmdline[i]) && cmdline[i] != '|' && cmdline[i] != '<' && cmdline[i] != '>')
                i++;
            int end = i;

            if (count == heredoc_index)
            {
                char *raw_delim = ft_substr(cmdline, start, end - start);
                int quoted = 0;

                if (raw_delim)
                {
                    size_t len = ft_strlen(raw_delim);
                    // Consider quoted if delimiter contains any quotes or is surrounded by quotes
                    if ((len >= 2 && ((raw_delim[0] == '\'' && raw_delim[len - 1] == '\'') ||
                                     (raw_delim[0] == '"' && raw_delim[len - 1] == '"'))) ||
                        ft_strchr(raw_delim, '\'') || ft_strchr(raw_delim, '"'))
                    {
                        quoted = 1;
                    }
                    free(raw_delim);
                }
                return quoted;
            }
            count++;
        }
        else
        {
            i++;
        }
    }

    return 0;
}


static char *strip_quotes(const char *str)
{
    size_t len = ft_strlen(str);

    if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') ||
                     (str[0] == '"' && str[len - 1] == '"')))
    {
        return ft_substr(str, 1, len - 2); // allocates a new string without quotes
    }
    return ft_strdup(str); // no quotes, just duplicate it
}


static int create_heredoc(char *delimiter, t_shell *shell, int heredoc_index)
{
    int     fd[2];
    char    *line;
    int     quoted;
    char    *real_delim;

    if (pipe(fd) == -1)
        return (-1);

    // Check if the delimiter was quoted in the command line
    quoted = is_delimiter_quoted_in_cmd(shell->rl_copy, delimiter, heredoc_index);
    real_delim = strip_quotes(delimiter);

    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, real_delim) == 0)
        {
            if (!line)
            {
                ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `",2);
                ft_putstr_fd(delimiter,2);
                ft_putstr_fd("\')\n", 2);
            }
            free(line);
            break;
        }

        if (!quoted)
        {
            char *expanded = replace_vars(line, shell);
            write(fd[1], expanded, ft_strlen(expanded));
            write(fd[1], "\n", 1);
            free(expanded);
        }
        else
        {
            write(fd[1], line, ft_strlen(line));
            write(fd[1], "\n", 1);
        }
        free(line);
    }

    free(real_delim);
    close(fd[1]);
    return (fd[0]);
}


static int process_in_heredoc(t_executor *current, t_lexer_list *lexer, t_shell *shell)
{
    // if (!lexer->next || lexer->next->type != EMPTY)
    // {
    //     ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
    //     return (FAILED);
    // }
    int heredoc_index = 0;
    if (lexer->type == HEREDOC)
    {
        current->fd_in = create_heredoc(lexer->next->str, shell, heredoc_index++);
        if (current->fd_in == -1)
            return (FAILED);
    }
    else
    {
        current->fd_in = open(lexer->next->str, O_RDONLY);
        if (current->fd_in == -1)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            perror(lexer->next->str);
            return (FAILED);
        }
    }
    return (OK);
}

static int process_command(t_executor *current, t_lexer_list *lexer)
{
    int size = 0;
    t_lexer_list *tmp = lexer;

    while (tmp && tmp->type == CMD)
    {
        size++;
        tmp = tmp->next;
    }

    current->execs = (char **)ft_calloc(size + 1, sizeof(char *));
    if (!current->execs)
        return (FAILED);

    for (int i = 0; i < size; i++)
    {
        current->execs[i] = ft_strdup(lexer->str);
        if (!current->execs[i])
        {
            free_str_arr(current->execs);
            return (FAILED);
        }
        lexer = lexer->next;
    }
    return (OK);
}

// static t_executor *process_lexemes(t_executor *list, t_executor *current, t_lexer_list **lexer)
// {
//     int ret;

//     while (*lexer)
//     {
//         if ((*lexer)->type == PIPE)
//         {
//             (*lexer) = (*lexer)->next;
//             break;
//         }
//         if ((*lexer)->type == REDERECT_OUT || (*lexer)->type == APPEND)
//         {
//             ret =  process_out_append(current, *lexer);
//             if (ret == FAILED)
//                 return (free_executor_list(list));
//             *lexer = (*lexer)->next->next;
//         }
//         else if ((*lexer)->type == REDERECT_IN || (*lexer)->type == HEREDOC)
//         {
//             ret = process_in_heredoc(current, *lexer);
//             if (ret == FAILED)
//                 return (free_executor_list(list));
//             *lexer = (*lexer)->next->next;
//         }
//         else if ((*lexer)->type == CMD)
//         {
//             ret = process_command(current, *lexer);
//             if (ret == FAILED)
//                 return (free_executor_list(list));
//             while (*lexer && (*lexer)->type == EMPTY)
//                 (*lexer) = (*lexer)->next;
//         }
//     }
//     return (list) ;////// just for handle return value warning
// }

static t_executor *process_lexemes(t_executor *list, t_executor *current, t_lexer_list **lexer, t_shell *shell)
{
    int ret;

    while (*lexer)
    {
        if ((*lexer)->type == PIPE)
        {
            (*lexer) = (*lexer)->next;
            break;
        }
        if ((*lexer)->type == REDERECT_OUT || (*lexer)->type == APPEND)
        {
            ret = process_out_append(current, *lexer);
            if (ret == FAILED)
                return (free_executor_list(list));
            *lexer = (*lexer)->next->next;
        }
        else if ((*lexer)->type == REDERECT_IN || (*lexer)->type == HEREDOC)
        {
            ret = process_in_heredoc(current, *lexer, shell);
            if (ret == FAILED)
                return (free_executor_list(list));
            *lexer = (*lexer)->next->next;
        }
        else if ((*lexer)->type == CMD)
        {
            ret = process_command(current, *lexer);
            if (ret == FAILED)
                return (free_executor_list(list));
            while (*lexer && (*lexer)->type == CMD)
                *lexer = (*lexer)->next;
        }
        else
        {
            *lexer = (*lexer)->next;
        }
    }
    return (list);
}


// t_executor *fill_executor_list(t_shell *shell, t_executor *list)
// {
//     t_executor  *current;
//     t_lexer_list    *lexer;

//     current = list;
//     lexer = shell->lex_head;
//     while (current)
//     {
//         // list = process_lexemes(list, current, &lexer);
//         if (!list)
//             return (NULL);
//         current = current->next;
//     }
//     return (list);
// }
t_executor *fill_executor_list(t_shell *shell, t_executor *list)
{
    t_executor      *current;
    t_lexer_list    *lexer;

    current = list;
    lexer = shell->lex_head;
    while (current)
    {
        list = process_lexemes(list, current, &lexer, shell);
        if (!list)
            return (NULL);
        current = current->next;
    }
    return (list);
}
