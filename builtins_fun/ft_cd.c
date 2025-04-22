/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:03:20 by selbouka          #+#    #+#             */
/*   Updated: 2025/04/22 15:29:32 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environ_node   *ft_getenv(t_environ_list *env, char *key)
{
    t_environ_node *current;
    
    current = env->head;
    while (current)
    {
        if (current->key && !ft_strcmp(current->key, key))
            return (current);
        current = current->next;
    }
    return (NULL);
}

int 	env_var_update(t_environ_list *env, char *key, char *value)
{
    t_environ_node *node;
    
    node = ft_getenv(env, key);
    if (node)
    {
        free(node->value);// allcooihm b malloc
        if (!(node->value = ft_strdup(value)))
            return(FAILED);
        return (OK);
    }
    node = malloc (sizeof(t_environ_node));
    if (!node)
        return (FAILED);
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
    if (!node->key || !node->key)
        return (/*free and*/FAILED);
    node->next = env->head;
    env->head = node;
    env->size++;
    return (OK);
}

int cd_no_args(t_shell *shell)
{
    t_environ_node     *home;
    char    old_pwd[PATH_MAX];
    if (!(home = ft_getenv(shell->env,"HOME")))
        return(ft_putstr_fd("cd: HOME not set\n", 2), FAILED);
    if (!getcwd(old_pwd, sizeof(old_pwd)))
        return(ft_putstr_fd("getcwd\n", 2), FAILED);
    if (chdir(home->value) != 0)
        return(ft_putstr_fd("Chdir\n", 2), FAILED);
    env_var_update(shell->env, "PWD", home->value);
    env_var_update(shell->env, "OLDPWD", old_pwd);
    return (OK);
}

int 	cd (t_shell *shell, char **arg)
{
    char    old_pwd[PATH_MAX];
    char    new_pwd[PATH_MAX];

    (void)shell;
    if (arg[2])
    {
        ft_putstr_fd("bash: cd: too many arguments\n", 2);
        return (FAILED);
    }
    if (!getcwd(old_pwd, sizeof(old_pwd)))
    {
        ft_putstr_fd("cd: error retrieving current directory\n", 2);
            return (FAILED);
    }
    if (!arg[1])
    {
        if (cd_no_args(shell) == FAILED)
            return (FAILED);
        return (OK);
    } 
    else if (chdir(arg[1]) != 0)
    {
        ft_putstr_fd("minishell: cd: ",2);
        ft_putstr_fd(arg[1],2);
        ft_putstr_fd(": No such file or directory\n",2);
        return (FAILED);
    }
    if (!getcwd(new_pwd, sizeof(new_pwd)))
    {
        ft_putstr_fd("cd: error retrieving new directory\n", 2);
            return (FAILED);
    }
    env_var_update(shell->env, "PWD", new_pwd);
    env_var_update(shell->env, "OLDPWD", old_pwd);
    
    return (OK);
    
}