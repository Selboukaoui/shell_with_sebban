/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:50:49 by asebban           #+#    #+#             */
/*   Updated: 2025/04/21 13:51:14 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	echo(char **args)
{
	int	i;
	int	newline;
	int	arg_count;

	i = 1;
	newline = 1;
	arg_count = 0;
	
	// Count arguments and check for -n flag
	while (args[arg_count])
		arg_count++;
	
	// Handle -n option (must be first argument and exactly "-n")
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	
	// Print all arguments separated by spaces
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	
	// Print newline unless -n was specified
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	
	return (EXIT_SUCCESS);
}