/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:03:20 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/06 09:59:49 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_environ_node	*ft_getenv(t_environ_list *env, char *key)
{
	t_environ_node	*current;
	
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
		free(node->value);
		node->value = ft_strdup1(value);
		free(value);
		return node->value ? OK : FAILED;
	}
	node = malloc (sizeof(t_environ_node));
	if (!node)
		return (FAILED);
	node->key = ft_strdup1(key);
	node->value = ft_strdup1(value);
	if (!node->key || !node->key)
		return (FAILED);
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
		return(ft_putstr_fd("getcwd\n", 2), FAILED); // ?
	if (chdir(home->value) != 0)
		return(ft_putstr_fd("Chdir\n", 2), FAILED);
	if(env_var_update(shell->env, "PWD", home->value) == FAILED)
		return (FAILED);
	if (env_var_update(shell->env, "OLDPWD", old_pwd) == FAILED)
		return (FAILED);
	return (OK);
}

int	cd(t_shell *shell, char **arg)
{
	char		old_pwd[PATH_MAX];
	char		new_pwd[PATH_MAX];
	static char	*x;
	// char *new_x ;
	if (arg[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (FAILED);
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		if (!ft_strcmp("..", arg[1]))
		{
			if (env_var_update(shell->env, "OLDPWD", ft_strjoin(old_pwd, x)) == FAILED)
				return (FAILED);
			x = ft_strjoin(x, "/..");
			if (env_var_update(shell->env, "PWD", ft_strjoin(old_pwd, x)) == FAILED)
				return (FAILED);
		}
		else if (!ft_strcmp(".", arg[1]))
		{
			if (env_var_update(shell->env, "OLDPWD", ft_strjoin(old_pwd, x)) == FAILED)
				return (FAILED);
			x = ft_strjoin(x, "/.");
			if (env_var_update(shell->env, "PWD", ft_strjoin(old_pwd, x)) == FAILED)
				return (FAILED);
		}
		chdir(arg[1]);
		if (getcwd(old_pwd, sizeof(old_pwd)))
		{
			if (env_var_update(shell->env, "PWD", old_pwd) == FAILED)
				return (FAILED);
			x = NULL;
		}
		ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 2);
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
	if ( env_var_update(shell->env, "PWD", new_pwd) == FAILED)
		return (FAILED);
	if (env_var_update(shell->env, "OLDPWD", old_pwd) == FAILED)
		return (FAILED);
	return (OK);
}
