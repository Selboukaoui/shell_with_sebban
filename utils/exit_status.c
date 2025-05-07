/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:00:30 by asebban           #+#    #+#             */
/*   Updated: 2025/05/07 11:25:15 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exit_status(int action, int value)
{
	static int	status;

	status = 0;
	if (action == 0)
		return (status);
	else if (action == 1)
		status = value;
	return (status);
}
int	herdoc_status(int action, int value)
{
	static int	status;

	// status = 0;
	if (action == 0)
		return (status);
	else if (action == 1)
		status = value;
	return (status);
}