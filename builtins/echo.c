/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:41:10 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 14:27:41 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	check_flags(t_cmd *cmd, int *i, int *newline)
{
	int	j;

	while (true)
	{
		if ((cmd->args && cmd->args[*i]) && (cmd->args[*i][0] == '-')
			&& (cmd->args[*i][1] == 'n'))
		{
			j = 1;
			while (cmd->args[*i][j] == 'n')
				j++;
			if (cmd->args[*i][j] == '\0')
			{
				*newline = 0;
				(*i)++;
			}
			else
				break ;
		}
		else
			break ;
	}
}

void	echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	check_flags(cmd, &i, &newline);
	if (!cmd->args || !cmd->args[1])
		return (ft_putchar_fd('\n', STDOUT_FILENO));
	while (cmd->args[i])
	{
		if (!cmd->args[i])
			break ;
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
}
