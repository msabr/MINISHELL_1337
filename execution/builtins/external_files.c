/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:17:16 by msabr             #+#    #+#             */
/*   Updated: 2025/07/19 15:20:25 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	execve_builtin(t_cmd *cmd, t_env **env_list)
{
	int	status;

	status = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		echo(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		pwd(env_list);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		status = cd(cmd, env_list);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		exit_shell(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		env_function(*env_list);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		status = export(cmd, env_list);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		status = unset(cmd, env_list);
	return (status);
}

bool	is_builtin(char *cmd)
{
	if (!cmd || cmd[0] == '\0')
		return (false);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0
		||ft_strcmp(cmd, "unset") == 0)
		return (true);
	return (false);
}
