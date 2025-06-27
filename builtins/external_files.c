/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:17:16 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 15:08:26 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	execve_builtin(char **args, t_env **env_list)
{
	t_cmd	cmd;

	cmd.args = args;
	if (ft_strcmp(args[0], "echo") == 0)
		echo(&cmd);
	else if (ft_strcmp(args[0], "pwd") == 0)
		pwd(env_list);
	else if (ft_strcmp(args[0], "cd") == 0)
		cd(&cmd, env_list);
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_shell(&cmd, *env_list);
	else if (ft_strcmp(args[0], "env") == 0)
		env_function(*env_list);
	else if (ft_strcmp(args[0], "export") == 0)
		export(&cmd, env_list);
	else if (ft_strcmp(args[0], "unset") == 0)
		unset(&cmd, env_list);
}

bool	is_bultins(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "exit") == 0
		|| ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0
		||ft_strcmp(cmd, "unset") == 0)
		return (true);
	return (false);
}
