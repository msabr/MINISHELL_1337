/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 20:57:43 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	exit_shell(t_cmd *cmd, t_env *env_list)
{
	int	exit_code;

	exit_code = 0;
	cmd->args++;
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
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return ;
	}
	free_env_list(env_list);
	exit(exit_code);
}
