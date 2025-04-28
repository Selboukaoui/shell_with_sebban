/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:11:51 by asebban           #+#    #+#             */
/*   Updated: 2025/04/28 10:11:42 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void clean_exit(t_shell *shell, int exit_code)
{
    (void)shell;
    //clean
    exit(exit_code);
}

static bool	is_arg_number(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!arg[i])
		return (false);
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

int exit_builtin(t_shell *shell, char **args)
{
    int exit_code;
    int arg_count;

    arg_count = 0;
    exit_code = exit_status(0, 0);
    while (args[arg_count])
        arg_count++;
    if (arg_count == 1)
        ( printf("exit\n"),clean_exit(shell, exit_code));
    if (!is_arg_number(args[1]))
	{
        printf("exit\n");
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		clean_exit(shell, 2);
	}
    if (arg_count > 2)
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
    }
    printf("exit\n");
    exit_code = ft_atoi(args[1]);
	clean_exit(shell, exit_code);
	return (EXIT_SUCCESS); // This line is theoretically unreachable
}