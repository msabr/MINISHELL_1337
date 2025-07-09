/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/09 15:12:18 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static bool	valid_number(const char *str)
{
	long long	num;
	int			sign;
	int			i;

	(1) && (num = 0, sign = 1, i = 0);
	if (!str || !str[0])
		return (false);
	if (!ft_is_number(str))
		return (false);
	if (str[0] == '+' || str[0] == '-')
	{
		if (str[0] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (num > ((long long)LONG_MAX - (str[i] - '0')) / 10)
			return (false);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (true);
}

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
		if (valid_number(cmd->args[1]))
			status = ft_atoi(cmd->args[1]) % 256;
		else
			print_exit_error(cmd->args[1]);
	}
	if (cmd->args[1] && cmd->args[2])
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2));
	ft_free();
	exit(status);
}
