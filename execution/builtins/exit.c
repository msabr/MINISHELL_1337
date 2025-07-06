/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 20:39:51 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	exit_shell(t_cmd *cmd)
{
	int	status;

	status = 0;
	if (!cmd->in_pipe)
	{
		restore_std_fds(cmd);
		ft_putstr_fd("exit\n", STDOUT_FILENO);
		save_std_fds(cmd);
	}
	if (cmd->args[1])
	{
		if (ft_is_number(cmd->args[1]))
			status = ft_atoi(cmd->args[1]) % 256;
		else
		{
			print_exit_error(cmd->args[1]);
			ft_free();
			exit(255);
		}
	}
	if (cmd->args[1] && cmd->args[2])
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2));
	ft_free();
	exit(status);
}
