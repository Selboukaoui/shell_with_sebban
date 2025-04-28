/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:28:53 by asebban           #+#    #+#             */
/*   Updated: 2025/04/28 12:12:50 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void print_sorted_env(t_environ_list *env)
{
	t_environ_node *tmp = env->head;
	t_environ_node *sorted = NULL;
	t_environ_node *current;
	t_environ_node *next;

	while (tmp)
	{
		char *full;
		if (tmp->value)
		{
			char *joined = ft_strjoin(tmp->key, "=");
			full = ft_strjoin(joined, tmp->value);
			free(joined);
		}
		else
		{
			full = ft_strdup(tmp->key);
		}

		current = create_environ_node(full);  // Assumes this splits properly into key/value
		free(full);
		if (!current)
			return;

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
		free(current->key);
		free(current->value);
		free(current);
	}
}

static int export_no_args(t_shell *shell)
{
	print_sorted_env(shell->env);
	return (EXIT_SUCCESS);
}

// static bool is_valid_identifier(char *str)
// {
// 	int i = 0;

// 	if (!str || !*str || ft_isdigit(str[0]) || str[0] == '=')
// 		return false;
// 	while (str[i] && str[i] != '=')
// 	{
// 		if (!ft_isalnum(str[i]) && str[i] != '_')
// 			return false;
// 		i++;
// 	}
// 	return true;
// }

static bool is_valid_identifier(char *str)
{
    int i = 0;

    if (!str || !*str || ft_isdigit(str[0]) || str[0] == '=')
        return false;

    while (str[i] && str[i] != '=')
    {
        if (str[i] == '+' && str[i + 1] == '=')
            return true; // allow '+' only if it's just before '='
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return false;
        i++;
    }
    return true;
}


void exporting(t_shell *shell, char *str)
{
    char *equal_sign = ft_strchr(str, '=');
    char *key;
    char *value;
    t_environ_node *node;
    bool append = false;

    if (equal_sign && equal_sign > str && *(equal_sign - 1) == '+')
    {
        append = true;
        key = ft_substr(str, 0, (equal_sign - str) - 1); // remove '+' from key
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
                char *new_value = ft_strjoin(node->value, value);
                free(node->value);
                node->value = new_value;
                free(value);
            }
            else
            {
                free(node->value);
                node->value = value;
            }
        }
        else
            free(value);
    }
    else
    {
        if (equal_sign)
        {
            char *temp = ft_strjoin(key, "=");
            char *full = ft_strjoin(temp, value);
            free(temp);
            node = create_environ_node(full);
            free(full);
        }
        else
        {
            node = ft_calloc(1, sizeof(t_environ_node));
            node->key = ft_strdup(key);
            node->value = NULL;
            node->next = NULL;
            printf("i was here\n");
        }
        add_back_environ_node(shell->env, node);
    }
    free(key);
}

// void exporting(t_shell *shell, char *str)
// {
// 	char *equal_sign = ft_strchr(str, '=');
// 	char *key;
// 	char *value;
// 	t_environ_node *node;

// 	if (equal_sign)
// 	{
// 		key = ft_substr(str, 0, equal_sign - str);
// 		value = ft_strdup(equal_sign + 1);
// 	}
// 	else
// 	{
// 		key = ft_strdup(str);
// 		value = NULL;
// 	}

// 	node = get_node(shell->env, key);
// 	if (node)
// 	{
// 		// If the export is without '=', don’t update value
// 		if (equal_sign)
// 		{
// 			free(node->value);
// 			node->value = value;
// 		}
// 		else
// 			free(value);
// 	}
// 	else
// 	{
// 		if (equal_sign)
// 		{
// 			char *temp = ft_strjoin(key, "=");
// 			char *full = ft_strjoin(temp, value);
// 			free(temp);
// 			node = create_environ_node(full);
// 			free(full);
// 		}
// 		else
// 		{
// 			// No '=': mark as exported, but no value (Bash behavior)
// 			node = ft_calloc(1, sizeof(t_environ_node));
// 			node->key = ft_strdup(key);
// 			node->value = NULL;
// 			node->next = NULL;
// 			printf("i was here\n");
// 		}
// 		add_back_environ_node(shell->env, node);
// 	}
// 	// free(key);
// 	// printf("this is the addtr%p\n", node->value);
// }

static int export_args(t_shell *shell, char **args)
{
	int i = 0;
	int ret = EXIT_SUCCESS;

	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = EXIT_FAILURE;
			continue;
		}
		exporting(shell, args[i]);
	}
	return ret;
}

int export(t_shell *shell, char **args)
{
	if (!args[1])
		return export_no_args(shell);
	return export_args(shell, args);
}
