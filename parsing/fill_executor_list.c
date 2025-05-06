/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_executor_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 16:18:37 by asebban           #+#    #+#             */
/*   Updated: 2025/05/06 17:42:31 by asebban          ###   ########.fr       */
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
    current->rederect_out = (lexer->type == rederect_out);
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

// static int is_delimiter_quoted_in_cmd(const char *cmdline, const char *delimiter, int heredoc_index)
// {
//     int i = 0;
//     int count = 0;
//     int in_single = 0, in_double = 0;

//     (void)delimiter;
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

//         if (!in_single && !in_double && cmdline[i] == '<' && cmdline[i + 1] == '<')
//         {
//             i += 2;
//             while (cmdline[i] && ft_isspace(cmdline[i]))
//                 i++;

//             // Extract delimiter token from the command line
//             int start = i;
//             while (cmdline[i] && !ft_isspace(cmdline[i]) && cmdline[i] != '|' && cmdline[i] != '<' && cmdline[i] != '>')
//                 i++;
//             int end = i;

//             if (count == heredoc_index)
//             {
//                 char *raw_delim = ft_substr(cmdline, start, end - start);
//                 int quoted = 0;

//                 if (raw_delim)
//                 {
//                     size_t len = ft_strlen(raw_delim);
//                     // Consider quoted if delimiter contains any quotes or is surrounded by quotes
//                     if ((len >= 2 && ((raw_delim[0] == '\'' && raw_delim[len - 1] == '\'') ||
//                                      (raw_delim[0] == '"' && raw_delim[len - 1] == '"'))) ||
//                         ft_strchr(raw_delim, '\'') || ft_strchr(raw_delim, '"'))
//                     {
//                         quoted = 1;
//                     }
//                     free(raw_delim);
//                 }
//                 return quoted;
//             }
//             count++;
//         }
//         else
//         {
//             i++;
//         }
//     }

//     return 0;
// }


// static char *strip_quotes(const char *str)
// {
//     size_t len = ft_strlen(str);

//     if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'') ||
//                      (str[0] == '"' && str[len - 1] == '"')))
//     {
//         return ft_substr(str, 1, len - 2); // allocates a new string without quotes
//     }
//     return ft_strdup(str); // no quotes, just duplicate it
// }

// char *get_last_heredoc_delimiter(const char *cmdline) {
//     char *last = NULL;

//     for (size_t i = 0; cmdline[i] != '\0'; ++i) {
//         if (cmdline[i] == '<' && cmdline[i+1] == '<') {
//             i += 2;
//             /* skip any spaces */
//             while (cmdline[i] && isspace((unsigned char)cmdline[i]))
//                 ++i;

//             /* start of delimiter */
//             size_t start = i;
//             while (cmdline[i] &&
//                    !isspace((unsigned char)cmdline[i]) &&
//                    cmdline[i] != '<' &&
//                    cmdline[i] != '>' &&
//                    cmdline[i] != '|') {
//                 ++i;
//             }
//             size_t len = i - start;
//             if (len > 0) {
//                 /* free previous, keep only the last */
//                 free(last);
//                 last = strndup(cmdline + start, len);
//             }
//         }
//     }
//     return last;
// }
// static int create_heredoc(char *delimiter, t_shell *shell, int heredoc_index)
// {
//     int     fd[2];
//     char    *line;
//     int     quoted;
//     char    *real_delim;

//     if (pipe(fd) == -1)
//         return (-1);

//     // Check if the delimiter was quoted in the command line
//     quoted = is_delimiter_quoted_in_cmd(shell->rl_copy, delimiter, heredoc_index);
//     real_delim = strip_quotes(delimiter);

//     while (1)
//     {
//         line = readline("> ");
//         if (!line || ft_strcmp(line, real_delim) == 0)
//         {
//             if (!line)
//             {
//                 ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `",2);
//                 ft_putstr_fd(delimiter,2);
//                 ft_putstr_fd("\')\n", 2);
//             }
//             free(line);
//             break;
//         }

//         if (!quoted)
//         {
//             char *expanded = replace_vars(line, shell);
//             write(fd[1], expanded, ft_strlen(expanded));
//             write(fd[1], "\n", 1);
//             free(expanded);
//         }
//         else
//         {
//             write(fd[1], line, ft_strlen(line));
//             write(fd[1], "\n", 1);
//         }
//         free(line);
//     }

//     free(real_delim);
//     close(fd[1]);
//     return (fd[0]);
// }


// static char *get_last_heredoc_delimiter(const char *cmdline)
// {
//     char *last = NULL;
//     for (size_t i = 0; cmdline[i]; ++i) {
//         if (cmdline[i] == '<' && cmdline[i + 1] == '<') {
//             i += 2;
//             while (cmdline[i] && ft_isspace(cmdline[i]))
//                 ++i;
//             size_t start = i;
//             while (cmdline[i] && !ft_isspace(cmdline[i])
//                    && cmdline[i] != '<' && cmdline[i] != '>' && cmdline[i] != '|')
//                 ++i;
//             size_t len = i - start;
//             if (len > 0) {
//                 free(last);
//                 last = strndup(cmdline + start, len);
//             }
//         }
//     }
//     return last;
// }

// /**
//  * Return 1 if the *last* heredoc delimiter in cmdline is wrapped in matching
//  * single or double quotes; 0 otherwise.
//  */
// static int is_last_delim_quoted(const char *cmdline)
// {
//     char *raw = get_last_heredoc_delimiter(cmdline);
//     if (!raw) return 0;
//     size_t n = ft_strlen(raw);
//     int quoted = (n >= 2 &&
//                  ((raw[0] == '\'' && raw[n-1] == '\'') ||
//                   (raw[0] == '"' && raw[n-1] == '"')));
//     free(raw);
//     return quoted;
// }

// /**
//  * Strip matching outer single or double quotes from s.
//  * Returns newly allocated unquoted string, or strdup(s) if no quotes.
//  */
// static char *strip_quotes(const char *s)
// {
//     size_t n = ft_strlen(s);
//     if (n >= 2 &&
//        ((s[0] == '\'' && s[n-1] == '\'') ||
//         (s[0] == '"' && s[n-1] == '"'))) {
//         return ft_substr(s, 1, n - 2);
//     }
//     return ft_strdup(s);
// }

// /**
//  * Create a heredoc: read lines until the given delimiter (quotes stripped).
//  * Variables are expanded only if this delimiter is NOT the last one that was quoted.
//  * Returns read-end FD of the pipe, or -1 on error.
//  */
// int create_heredoc(const char *delimiter, t_shell *shell)
// {
//     int pipefd[2];
//     if (pipe(pipefd) == -1)
//         return -1;

//     int quoted = is_last_delim_quoted(shell->rl_copy);
//     char *real_delim = strip_quotes(delimiter);

//     while (1) {
//         char *line = readline("> ");
//         if (!line || ft_strcmp(line, real_delim) == 0) {
//             free(line);
//             break;
//         }
//         if (!quoted) {
//             char *expanded = replace_vars(line, shell);
//             write(pipefd[1], expanded, ft_strlen(expanded));
//             write(pipefd[1], "\n", 1);
//             free(expanded);
//         } else {
//             write(pipefd[1], line, ft_strlen(line));
//             write(pipefd[1], "\n", 1);
//         }
//         free(line);
//     }

//     free(real_delim);
//     close(pipefd[1]);
//     return pipefd[0];
// }

// /**
//  * Process input redirection: if HEREDOC, invoke create_heredoc with the
//  * raw token after HEREDOC; otherwise open filename. Returns OK or FAILED.
//  */
// static int process_in_heredoc(t_executor *cur, t_lexer_list *lex, t_shell *sh)
// {
//     if (lex->type == HEREDOC) {
//         cur->fd_in = create_heredoc(lex->next->str, sh);
//         if (cur->fd_in == -1)
//             return FAILED;
//     } else {
//         cur->fd_in = open(lex->next->str, O_RDONLY);
//         if (cur->fd_in == -1) {
//             ft_putstr_fd("minishell: ", STDERR_FILENO);
//             perror(lex->next->str);
//             return FAILED;
//         }
//     }
//     return OK;
// }


// static int process_in_heredoc(t_executor *current, t_lexer_list *lexer, t_shell *shell)
// {
//     // if (!lexer->next || lexer->next->type != EMPTY)
//     // {
//     //     ft_putstr_fd("minishell: syntax error near unexpected token\n", STDERR_FILENO);
//     //     return (FAILED);
//     // }
//     int heredoc_index = 0;
//     if (lexer->type == HEREDOC)
//     {
//         current->fd_in = create_heredoc(lexer->next->str, shell, heredoc_index++);
//         if (current->fd_in == -1)
//             return (FAILED);
//     }
//     else
//     {
//         current->fd_in = open(lexer->next->str, O_RDONLY);
//         if (current->fd_in == -1)
//         {
//             ft_putstr_fd("minishell: ", STDERR_FILENO);
//             perror(lexer->next->str);
//             return (FAILED);
//         }
//     }
//     return (OK);
// }

static char *get_last_heredoc_delimiter(const char *cmdline)
{
    char *last = NULL;
    for (size_t i = 0; cmdline[i]; ++i) {
        if (cmdline[i] == '<' && cmdline[i + 1] == '<') {
            i += 2;
            while (cmdline[i] && ft_isspace(cmdline[i]))
                ++i;
            size_t start = i;
            while (cmdline[i] && !ft_isspace(cmdline[i])
                   && cmdline[i] != '<' && cmdline[i] != '>' && cmdline[i] != '|')
                ++i;
            size_t len = i - start;
            if (len > 0) {
                // free(last);
                last = ft_strndup(cmdline + start, len); // check strndup
            }
        }
    }
    return last;
}

/**
 * Return 1 if the *last* heredoc delimiter in cmdline contains any quote
 * characters (single or double), indicating it was quoted; 0 otherwise.
 */
static int is_last_delim_quoted(const char *cmdline)
{
    char *raw = get_last_heredoc_delimiter(cmdline);
    if (!raw)
        return 0;
    int quoted = (strchr(raw, '\'') != NULL || strchr(raw, '"') != NULL);
    // free(raw);
    return quoted;
}

/**
 * Strip all single and double quotes from the delimiter token.
 * Returns newly allocated unquoted string, or strdup(s) if no quotes.
 */
// static char *strip_quotes(const char *s)
// {
//     size_t len = ft_strlen(s);
//     char *out = ft_malloc(len + 1);
//     if (!out) return NULL;
//     size_t j = 0;
//     for (size_t i = 0; i < len; ++i) {
//         if (s[i] != '\'' && s[i] != '"')
//             out[j++] = s[i];
//     }
//     out[j] = '\0';
//     return out;
// }


static char *replace_vars_heredoc(char *input, t_shell *shell)
{
    size_t  i = 0, j = 0, len = ft_strlen(input);
    char    *output;
    int     in_single_quote   = 0;
    int     in_double_quote   = 0;
    int     seen_export       = 0;
    int     in_export_assign  = 0;
    int     k;

    // detect leading “export …”
    {
        size_t p = 0;
        while (p < len && ft_isspace(input[p]))
            p++;
        if (p + 6 <= len
            && ft_strncmp(&input[p], "export", 6) == 0
            && (p + 6 == len || ft_isspace(input[p + 6])))
        {
            seen_export = 1;
        }
    }

    output = ft_malloc(PATH_MAX + 1, 1);
    if (!output)
        return NULL;

    while (i < len)
    {
        if (seen_export && !in_export_assign
            && input[i] == '=')
        {
            in_export_assign = 1;
            output[j++] = input[i++];
            continue;
        }

        if (in_export_assign
            && (ft_isspace(input[i]) || input[i] == '|'))
        {
            in_export_assign = 0;
            output[j++] = input[i++];
            continue;
        }

        if (input[i] == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote, output[j++] = input[i++];
        else if (input[i] == '"' && !in_single_quote)
            in_double_quote = !in_double_quote, output[j++] = input[i++];

        // Allow variable expansion even inside single quotes
        else if (input[i] == '$')
        {
            if (input[i + 1] == '\0')
            {
                output[j++] = '$';
                i++;
            }
            else if (input[i + 1] == '?')
            {
                if (seen_export && in_export_assign)
                    output[j++] = '"';

                int  status = exit_status(0, 0);
                char status_str[12];
                int_to_str(status, status_str);
                k = 0;
                while (status_str[k])
                    output[j++] = status_str[k++];
                i += 2;

                if (seen_export && in_export_assign)
                    output[j++] = '"';
            }
            else if (ft_isdigit(input[i + 1]))
            {
                i += 2;
            }
            else if (ft_isalpha(input[i + 1]) || input[i + 1] == '_')
            {
                if (seen_export && in_export_assign)
                    output[j++] = '"';

                int  var_start = i + 1;
                int  var_len   = 0;
                char var_name[256];

                while (ft_isalnum(input[var_start + var_len]) || input[var_start + var_len] == '_')
                    var_len++;

                ft_strncpy(var_name, &input[var_start], var_len);
                var_name[var_len] = '\0';

                char *val = get_env_value(shell->env, var_name);
                if (val)
                {
                    k = 0;
                    while (val[k])
                        output[j++] = val[k++];
                }
                i += var_len + 1;

                if (seen_export && in_export_assign)
                    output[j++] = '"';
            }
            else
            {
                output[j++] = '$';
                i++;
            }
            continue;
        }
        else
        {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
    return output;
}


/**
 * Create a heredoc: read lines until the given delimiter (quotes stripped).
 * Variables are expanded only if the *last* delimiter was not quoted.
 * Prints warning on EOF (Ctrl-D) and returns read-end FD, or -1 on error.
 */
int create_heredoc(char *delimiter, t_shell *shell)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;

    int quoted = is_last_delim_quoted(shell->rl_copy);
    // char *real_delim = strip_quotes(delimiter);
    // if (!real_delim)
    //     return -1;
    // printf("delemetre is :%s\n", delimiter);
    // printf("real_delemetre is :%s\n", real_delim);
    signal_setup(3);
    while (1) {
        char *line = readline("> ");
        if (!line) {
            // Ctrl-D pressed
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
            ft_putstr_fd(delimiter, STDERR_FILENO);
            ft_putstr_fd("')\n", STDERR_FILENO);
            break;
        }
        if (ft_strcmp(line, delimiter) == 0) {
            free(line);
            break;
        }
        if (!quoted) {
            // char *expanded = replace_vars(line, shell); thsi case : << c cat
            // > '$HOME'
            // > "'$PWD'"
            // > c
            char *expanded = replace_vars_heredoc(line, shell);
            write(pipefd[1], expanded, ft_strlen(expanded));
            write(pipefd[1], "\n", 1);
            free(expanded);
        } else {
            write(pipefd[1], line, ft_strlen(line));
            write(pipefd[1], "\n", 1);
        }
        free(line);
    }

    // free(real_delim);
    close(pipefd[1]);
    return pipefd[0];
}

/**
 * Process input redirection: if HEREDOC, invoke create_heredoc with the
 * raw token after HEREDOC; otherwise open filename. Returns OK or FAILED.
 */
static int process_in_heredoc(t_executor *cur, t_lexer_list *lex, t_shell *sh)
{
    if (lex->type == HEREDOC) {
        cur->fd_in = create_heredoc(lex->next->str, sh);
        if (cur->fd_in == -1)
            return FAILED;
    } else {
        cur->fd_in = open(lex->next->str, O_RDONLY);
        if (cur->fd_in == -1) {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            perror(lex->next->str);
            return FAILED;
        }
    }
    return OK;
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
            // free_str_arr(current->execs);
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
        if ((*lexer)->type == rederect_out || (*lexer)->type == APPEND)
        {
            ret = process_out_append(current, *lexer);
            if (ret == FAILED)
                return (/*free_executor_list(list)*/NULL);
            *lexer = (*lexer)->next->next;
        }
        else if ((*lexer)->type == REDERECT_IN || (*lexer)->type == HEREDOC)
        {
            ret = process_in_heredoc(current, *lexer, shell);
            if (ret == FAILED)
                return (/*free_executor_list(list)*/NULL);
            *lexer = (*lexer)->next->next;
        }
        else if ((*lexer)->type == CMD)
        {
            ret = process_command(current, *lexer);
            if (ret == FAILED)
                return (/*free_executor_list(list)*/NULL);
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
        {
            exit_status(1,1);
            return (NULL);
        }
        exit_status(1,0);
        current = current->next;
    }
    return (list);
}
