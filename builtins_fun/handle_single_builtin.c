/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_builtin.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:45:27 by asebban           #+#    #+#             */
/*   Updated: 2025/04/28 12:19:47 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

bool    is_builtin(char *exec)
{
    if (!exec)
        return (false);
    
    return (ft_strcmp(exec, "echo") == 0 ||
        ft_strcmp(exec, "cd") == 0 ||
        ft_strcmp(exec, "pwd") == 0 ||
        ft_strcmp(exec, "export") == 0 ||
        ft_strcmp(exec, "unset") == 0 ||
        ft_strcmp(exec, "env") == 0 ||
        ft_strcmp(exec, ":") == 0 ||
        ft_strcmp(exec, "exit") == 0);
}

static void saving_stds(int *stdin_cpy, int *stdout_cpy)
{
    *stdin_cpy = dup(STDIN_FILENO);
    *stdout_cpy = dup(STDOUT_FILENO);
    if (*stdin_cpy == -1 || *stdout_cpy == -1)
        perror("minishell: dup");
}

static void execute_builtin(t_shell *shell)
{
    char **args = shell->executor->execs;
    int exit_code;

    
    exit_code = 0;
    if (ft_strcmp(args[0], "pwd") == 0)
        exit_code = pwd(shell->env, 0);
    else if (ft_strcmp(args[0], "cd") == 0)
        exit_code = cd(shell, args);
    else if (ft_strcmp(args[0], ":") == 0)
        exit_code = 0;
    else if (ft_strcmp(args[0], "env") == 0)
        exit_code = env(shell);
    else if (ft_strcmp(args[0], "echo") == 0)
        exit_code = echo(args);
    else if (ft_strcmp(args[0], "exit") == 0)
        exit_code = exit_builtin(shell, args);
    else if (ft_strcmp(args[0], "export") == 0)
        exit_code = export(shell, args);
    else if (ft_strcmp(args[0], "unset") == 0)
        exit_code = unset(shell, args);
    exit_status(EXIT_SET, exit_code);
}

static bool handle_redirections_single_builtin(t_shell *shell)
{
    t_executor *exec = shell->executor;

    if (exec->fd_in != STDIN_FILENO)
    {
        if (dup2(exec->fd_in, STDIN_FILENO) == -1)
        {
            perror("minishell: dup2");
            return (false);
        }
        close(exec->fd_in);
    }

    if (exec->fd_out != STDOUT_FILENO)
    {
        if (dup2(exec->fd_out, STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2");
            return (false);
        }
        close(exec->fd_out);
    }
    return (true);
}

static bool restore_stds(int stdin_cpy, int stdout_cpy)
{
    bool success;

    success = true;
    if (dup2(stdin_cpy, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2");
        success = false;
    }
    if (dup2(stdout_cpy, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2");
        success = false;
    }
    close(stdin_cpy);
    close(stdout_cpy);
    return (success);
}

int handle_single_builtin(t_shell *shell)
{
    int stdin_copy;
    int stdout_copy;
    int ret;

    if (!handle_redirections_single_builtin(shell))
        return (EXIT_FAILURE);
    
    saving_stds(&stdin_copy, &stdout_copy);
    execute_builtin(shell);
    ret = restore_stds(stdin_copy, stdout_copy);
    
    if (ret)
        return(EXIT_SUCCESS);
    return(EXIT_FAILURE);

}