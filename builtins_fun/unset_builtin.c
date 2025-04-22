/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:32:39 by asebban           #+#    #+#             */
/*   Updated: 2025/04/22 15:13:36 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	is_valid_identifier(char *arg)
{
	if (!arg || !*arg || (!ft_isalpha(*arg) && *arg != '_'))
		return (false);
	while (*arg)
	{
		if (!ft_isalnum(*arg) && *arg != '_')
			return (false);
		arg++;
	}
	return (true);
}

static void	delete_variable(t_shell *shell, char *arg)
{
	t_environ_node	*current;
	t_environ_node	*prev;

	prev = NULL;
	current = shell->env->head;
	while (current)
	{
		if (ft_strcmp(current->key, arg) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env->head = current->next;
			free(current->key);
			free(current->value);
			free(current);
			shell->env->size--;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static void	unset_arg(t_shell *shell, char *arg, int *c)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		(*c)++;
		return ;
	}
	delete_variable(shell, arg);
}

int	unset(t_shell *shell, char **args)
{
	int	i;
	int	c;

	if (!args[1])
		return (EXIT_SUCCESS);
	c = 0;
	i = 1;
	while (args[i])
	{
		unset_arg(shell, args[i], &c);
		i++;
	}
    if(c)
        return(EXIT_FAILURE);
	return (EXIT_SUCCESS);
}