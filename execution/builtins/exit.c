/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/01 19:07:38 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


void	exit_shell(t_cmd *cmd)
{
	int	exit_code;

	exit_code = 0;
	cmd->args++;
	if (!cmd->args[0] && !cmd->args[1])
		return (ft_putendl_fd("exit", STDOUT_FILENO));
	ft_putendl_fd("exit", STDERR_FILENO);
	if (cmd->args[0])
	{
		if (ft_is_number(cmd->args[0]) == 0)
		{
			ft_putstr_fd("exit: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			exit_code = 255;
		}
		else
			exit_code = ft_atoi(cmd->args[0]);
	}
	if (cmd->args[0] && cmd->args[1] && exit_code != 255)
		return (ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO));
	exit(exit_code);
}
