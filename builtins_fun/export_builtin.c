/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selbouka <selbouka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:28:53 by asebban           #+#    #+#             */
/*   Updated: 2025/05/09 15:00:06 by selbouka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_sorted_env(t_environ_list *env)
{
	t_environ_node	*tmp;
	t_environ_node	*sorted;
	t_environ_node	*current;
	t_environ_node	*next;
	char			*full;
	char			*joined;

	tmp = env->head;
	sorted = NULL;
	while (tmp)
	{
		if (tmp->value)
		{
			joined = ft_strjoin(tmp->key, "=");
			full = ft_strjoin(joined, tmp->value);
		}
		else
			full = ft_strdup(tmp->key);
		current = create_environ_node(full);
		if (!current)
			return ;
		if (!sorted || ft_strcmp(current->key, sorted->key) < 0)
		{
			current->next = sorted;
			sorted = current;
		}
		else
		{
			next = sorted;
			while (next->next && ft_strcmp(current->key, next->next->key) > 0)
				next = next->next;
			current->next = next->next;
			next->next = current;
		}
		tmp = tmp->next;
	}
	while (sorted)
	{
		if (sorted->value)
			printf("declare -x %s=\"%s\"\n", sorted->key, sorted->value);
		else
			printf("declare -x %s\n", sorted->key);
		current = sorted;
		sorted = sorted->next;
	}
}

void	exporting(t_shell *shell, char *str)
{
	char			*equal_sign;
	char			*key;
	char			*value;
	t_environ_node	*node;
	bool			append;
	char			*temp;
	char			*full;
	char			*new_value;

	equal_sign = ft_strchr(str, '=');
	append = false;
	if (equal_sign && equal_sign > str && *(equal_sign - 1) == '+')
	{
		append = true;
		key = ft_substr(str, 0, (equal_sign - str) - 1);
		value = ft_strdup(equal_sign + 1);
	}
	else if (equal_sign)
	{
		key = ft_substr(str, 0, equal_sign - str);
		value = ft_strdup(equal_sign + 1);
	}
	else
	{
		key = ft_strdup(str);
		value = NULL;
	}
	node = get_node(shell->env, key);
	if (node)
	{
		if (equal_sign)
		{
			if (append && node->value)
			{
				new_value = ft_strjoin(node->value, value);
				node->value = new_value;
			}
			else
				node->value = value;
		}
	}
	else
	{
		if (equal_sign)
		{
			temp = ft_strjoin(key, "=");
			full = ft_strjoin(temp, value);
			node = create_environ_node(full);
		}
		else
		{
			node = ft_malloc(sizeof(t_environ_node), 1);
			node->key = ft_strdup(key);
			node->value = NULL;
			node->next = NULL;
		}
		add_back_environ_node(shell->env, node);
	}
}

static	int	export_args(t_shell *shell, char **args)
{
	int	i;
	int	ret;

	i = 0;
	ret = EXIT_SUCCESS;
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = EXIT_FAILURE;
			continue ;
		}
		exporting(shell, args[i]);
	}
	return (ret);
}

int	export(t_shell *shell, char **args)
{
	if (!args[1])
		return (export_no_args(shell));
	return (export_args(shell, args));
}
