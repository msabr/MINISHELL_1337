/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/29 17:32:29 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	print_exit_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	ft_free();
	ft_set_status(255);
	ft_exit(255);
}

static int	get_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

static bool	valid_number(const char *str)
{
	unsigned long long	num;
	int					sign;
	int					i;

	(1) && (num = 0, i = 0);
	if (!str || !str[i] || !ft_is_number(str))
		return (false);
	while (str[i] && ft_isspace(str[i]))
		i++;
	sign = get_sign(str, &i);
	if (!str[i])
		return (false);
	while (str[i])
	{
		if ((num > ((unsigned long long)LONG_MAX - (str[i] - '0')) / 10)
			&& (sign == 1))
			return (false);
		if ((num > ((unsigned long long)LONG_MAX + 1 - (str[i] - '0')) / 10)
			&& (sign == -1))
			return (false);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (true);
}

int	exit_shell(t_cmd *cmd)
{
	int	status;

	status = *ft_get_status();
	if (!cmd->in_pipe)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (cmd->args[1])
	{
		if (cmd->args[1][0] && valid_number(cmd->args[1]))
			status = ft_atoi(cmd->args[1]) % 256;
		else
			print_exit_error(cmd->args[1]);
	}
	if (cmd->args[1] && cmd->args[2])
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1);
	ft_free();
	ft_set_and_exit(status);
	return (status);
}
