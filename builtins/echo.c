/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:41:10 by msabr             #+#    #+#             */
/*   Updated: 2025/06/15 16:35:20 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void check_flags(t_cmd *cmd, int *i, int *newline)
{
	if (cmd->args && cmd->args[1] && cmd->args[1][0] == '-' && cmd->args[1][1] == 'n') 
	{
		int j = 1;
		while (cmd->args[1][j] == 'n')
			j++;
		if (cmd->args[1][j] == '\0')
		{
			*newline = 0;
			*i = 2;
		}
	} 
	else 
	{
		*i = 1;
	}
}

void echo(t_cmd *cmd)
{
	int i = 1;
	int newline = 1;

	check_flags(cmd, &i, &newline);
	if (!cmd->args || !cmd->args[1])
		return;
	while (cmd->args[i]) 
	{
		if (!cmd->args[i])
			break;
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
