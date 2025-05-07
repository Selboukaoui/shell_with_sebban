/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:03:20 by selbouka          #+#    #+#             */
/*   Updated: 2025/05/07 19:07:58 by selbouka         ###   ########.fr       */
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

int	env_var_update(t_environ_list *env, char *key, const char *value)
{
	t_environ_node	*node;

	node = ft_getenv(env, key);
	if (node)
	{
		node->value = ft_strdup(value);
		if (node->value)
			return (OK);
		return (FAIL);
	}
	node = ft_malloc (sizeof(t_environ_node), 1);
	if (!node)
		return (FAIL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	if (!node->key || !node->value)
		return (FAIL);
	node->next = env->head;
	env->head = node;
	env->size++;
	return (OK);
}

int	cd_no_args(t_shell *shell)
{
	t_environ_node	*home;
	char			old_pwd[PATH_MAX];

	home = ft_getenv(shell->env, "HOME");
	if (!home)
		return (ft_putstr_fd("cd: HOME not set\n", 2), FAIL);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (ft_putstr_fd("getcwd\n", 2), FAIL);
	if (chdir(home->value) != 0)
		return (ft_putstr_fd("Chdir\n", 2), FAIL);
	if (!env_var_update(shell->env, "PWD", home->value))
		return (FAIL);
	if (!env_var_update(shell->env, "OLDPWD", old_pwd))
		return (FAIL);
	return (OK);
}

int	cd(t_shell *shell, char **arg)
{
	static char		old_pwd[PATH_MAX];
	static char		new_pwd[PATH_MAX];
	static char		*x;

	if (arg[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), FAIL);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		if (!ft_strcmp("..", arg[1]))
		{
			if (!env_var_update(shell->env, "OLDPWD", ft_strjoin(old_pwd, x)))
				return (FAIL);
			x = ft_strjoin(x, "/..");
			if (!env_var_update(shell->env, "PWD", ft_strjoin(old_pwd, x)))
				return (FAIL);
		}
		else if (!ft_strcmp(".", arg[1]))
		{
			if (!env_var_update(shell->env, "OLDPWD", ft_strjoin(old_pwd, x)))
				return (FAIL);
			x = ft_strjoin(x, "/.");
			if (!env_var_update(shell->env, "PWD", ft_strjoin(old_pwd, x)))
				return (FAIL);
		}
		chdir(arg[1]);
		if (getcwd(old_pwd, sizeof(old_pwd)))
		{
			if (!env_var_update(shell->env, "PWD", old_pwd))
				return (FAIL);
			if (!env_var_update(shell->env, "OLDPWD", ft_strjoin(old_pwd, x)))
				return (FAIL);
			x = NULL;
		}
		return (ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot \
access parent directories: No such file or directory\n", 2), FAIL);
	}
	if (!arg[1])
	{
		if (cd_no_args(shell) == FAIL)
			return (FAIL);
		return (OK);
	}
	else if (chdir(arg[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(arg[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (FAIL);
	}
	if (!getcwd(new_pwd, sizeof(new_pwd)))
		return (ft_putstr_fd("cd: error retrieving new directory\n", 2), FAIL);
	if (!env_var_update(shell->env, "PWD", new_pwd))
		return (FAIL);
	if (!env_var_update(shell->env, "OLDPWD", old_pwd))
		return (FAIL);
	return (OK);
}
