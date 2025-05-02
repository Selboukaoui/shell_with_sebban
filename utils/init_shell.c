/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:42:18 by asebban           #+#    #+#             */
/*   Updated: 2025/05/02 14:33:44 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environ_list *empty_environ(t_environ_list *env)
{
    char    cwd[PATH_MAX];
    char    *pwd;
    char    *shlvl;
    char *PATH;
    char *mini;

    if (!getcwd(cwd, PATH_MAX))
    {
        perror("minishell: getcwd");
        return (NULL);
    }

    pwd = ft_strjoin("PWD=", cwd);
    shlvl = ft_strdup("SHLVL=1");
    PATH = ft_strdup("PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
    mini = ft_strdup("_=/home/asebban/Desktop/shell_with_selbouka/./minishell");
    if (!pwd || !shlvl || !PATH || !mini || !add_back_environ_node(env, create_environ_node(pwd)) ||
        !add_back_environ_node(env, create_environ_node(shlvl)) ||
    !add_back_environ_node(env, create_environ_node(mini)) || !add_back_environ_node(env, create_environ_node(PATH)))
    {
        //free(pwd);
        //free(shlvl);
        return (NULL);
    }
    // env->IS_NUUL = true;
    //free(pwd);
    //free(shlvl);
    return (env);
}

t_environ_list *init_environ(char **envp)
{
    t_environ_list  *env;
    t_environ_node  *node;
    char            *env_var;

    env = (t_environ_list *)ft_calloc(1, sizeof(t_environ_list));
    if (!env)
        return (NULL);

    if (!*envp)
        return (empty_environ(env));

    while (*envp)
    {
        env_var = ft_strdup(*envp);
        if (!env_var)
        {
            free_environ(env);
            return (NULL);
        }
        node = create_environ_node(env_var);
        if (!node)
        {
            //free(env_var);
            free_environ(env);
            return (NULL);
        }
        add_back_environ_node(env, node);
        envp++;
    }
    // env->IS_NUUL= false;
    return (env);
}


t_shell *init_shell(char **env)
{
    t_shell *shell;

    shell = (t_shell *)ft_calloc(1, sizeof(t_shell));
    if (!shell)
        return (NULL /*alloc failed*/);
    shell->env = init_environ(env);
    if (!shell->env)
    {
        //free(shell);
        return (NULL);
    }
    shell->lex_head = NULL;
    shell->executor = NULL;
    return (shell);
}
