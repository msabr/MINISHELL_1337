/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/02 18:04:39 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	exit_shell(t_cmd *cmd)
{
	int status = 0;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		exit(0);
	}
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (cmd->args[1])
	{
		if (ft_isdigit(cmd->args[1][0]) && ft_strlen(cmd->args[1]) == 1)
			status = ft_atoi(cmd->args[1]);
		else
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			return;
		}
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return;
	}
	exit(status);
}

