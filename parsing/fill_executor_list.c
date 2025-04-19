/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_executor_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:37 by asebban           #+#    #+#             */
/*   Updated: 2025/04/19 11:15:36 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool open_outputfile(t_executor *current, t_lexer_list *lexer)
{
    if (current->append)
        current->fd_out = open(lexer->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        current->fd_out = open(lexer->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (current->fd_out == -1)
    {
        // ft_putstr_fd("minishell: ", STDERR_FILENO);
        // perror(lexer->str);
        // return (false);
    }
    return (true);
}

static int process_out_append(t_executor *current, t_lexer_list *lexer)
{
    if (!lexer->next || lexer->next->type != EMPTY)
    {
        // ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
        return (FAILED);
    }
    
    current->append = (lexer->type == APPEND);
    current->REDERECT_OUT = (lexer->type == REDERECT_OUT);
    
    if (!open_outputfile(current, lexer->next))
        return (FAILED);
    
    return (OK);
}

static int create_heredoc(char *delimiter)
{
    int     fd[2];
    char    *line;

    if (pipe(fd) == -1)
        return (-1);
    
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd[1], line, ft_strlen(line));
        write(fd[1], "\n", 1);
        free(line);
    }
    close(fd[1]);
    return (fd[0]);
}


static int process_in_heredoc(t_executor *current, t_lexer_list *lexer)
{
    if (!lexer->next || lexer->next->type != EMPTY)
    {
        ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
        return (FAILED);
    }
    
    if (lexer->type == HEREDOC)
    {
        current->fd_in = create_heredoc(lexer->next->str);
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

static t_executor *process_lexemes(t_executor *list, t_executor *current, t_lexer_list **lexer)
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
            ret = process_in_heredoc(current, *lexer);
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
        list = process_lexemes(list, current, &lexer);
        if (!list)
            return (NULL);
        current = current->next;
    }
    return (list);
}
