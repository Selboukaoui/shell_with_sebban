/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:50:49 by asebban           #+#    #+#             */
/*   Updated: 2025/05/10 15:16:52 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	is_n_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i] == 'n')
		i++;
	return (str[i] == '\0');
}

int	total_len(char **args)
{
	int	len;

	len = 0;
	while (*args)
	{
		len += ft_strlen(*args); // + ' '
		args++; //check
	}
	return (len);
}

void	full_buff(char *arg, char *buff, int *j)
{
	int		i;

	i = 0;
	while (arg[i])
	{
		buff[*j] = arg[i];
		i++;
		(*j)++;
	}
}

int	echo(char **args)
{
	int			i;
	int			newline;
	char		*buff;
	size_t		len;
	static int	j;

	len = total_len(args);
	buff = ft_malloc (len + 1, 1); // check
	i = 1;
	newline = 1;
	while (args[i] && is_n_option(args[i]))
		1 && (newline = 0, i++);
	while (args[i])
	{
		full_buff(args[i], buff, &j);
		if (args[i + 1])
			full_buff(" ", buff, &j);
		i++;
	}
	buff[j] = '\0';
	printf ("%s", buff);
	if (newline)
		printf ("\n");
	j = 0;
	return (ft_bzero(buff, ft_strlen (buff)), EXIT_SUCCESS);
}
