/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:42:18 by asebban           #+#    #+#             */
/*   Updated: 2025/04/17 20:54:36 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environ_list *empty_environ(t_environ_list *environ)
{
    char    cwd[PATH_MAX];
    char    *pwd;
    char    *shlvl;

    if (!getcwd(cwd, PATH_MAX))
    {
        perror("minishell: getcwd");
        return (NULL);
    }

    pwd = ft_strjoin("PWD=", cwd);
    shlvl = ft_strdup("SHLVL=1");

    if (!pwd || !shlvl || !add_back_environ_node(environ, create_environ_node(pwd)) ||
        !add_back_environ_node(environ, create_environ_node(shlvl)))
    {
        free(pwd);
        free(shlvl);
        return (NULL);
    }

    free(pwd);
    free(shlvl);
    return (environ);
}

t_environ_list *init_environ(char **envp)
{
    t_environ_list  *environ;
    t_environ_node  *node;
    char            *env_var;

    environ = (t_environ_list *)ft_calloc(1, sizeof(t_environ_list));
    if (!environ)
        return (NULL);

    if (!*envp)
        return (empty_environ(environ));

    while (*envp)
    {
        env_var = ft_strdup(*envp);
        if (!env_var)
        {
            free_environ(environ);
            return (NULL);
        }
        node = create_environ_node(env_var);
        if (!node)
        {
            free(env_var);
            free_environ(environ);
            return (NULL);
        }
        add_back_environ_node(environ, node);
        envp++;
    }
    return (environ);
}


t_shell *init_shell(char **env)
{
    t_shell *shell;

    shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
    if (!shell)
        return (NULL /*alloc failed*/);
    shell->environ = init_environ(env);
    if (!shell->environ)
    {
        free(shell);
        return (NULL);
    }
    shell->lex_head = NULL;
    shell->executor = NULL;
    return (shell);
}
