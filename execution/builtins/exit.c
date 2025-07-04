/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 17:21:52 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	exit_shell(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (isatty(STDIN_FILENO))
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO);
	}
	if (cmd->args[1])
	{
		if (ft_isdigit(cmd->args[1][0]) && ft_strlen(cmd->args[1]) == 1)
			status = ft_atoi(cmd->args[1]);
		else
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit(255);
		}
	}
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return ;
	}
	exit(status);
}
