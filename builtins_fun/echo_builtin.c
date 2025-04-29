/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:50:49 by asebban           #+#    #+#             */
/*   Updated: 2025/04/29 10:41:17 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_n_option(const char *str)
{
	int i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0'); // valid if only '-' followed by one or more 'n'
}

int echo(char **args)
{
	int i = 1;
	int newline = 1;

	// Handle multiple -n options
	while (args[i] && is_n_option(args[i]))
	{
		newline = 0;
		i++;
	}

	// Print arguments
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}

	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);

	return (EXIT_SUCCESS);
}
