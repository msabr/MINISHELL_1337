/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:51:05 by msabr             #+#    #+#             */
/*   Updated: 2025/07/22 18:03:44 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	cd(t_cmd *cmd, t_env **env_list)
{
	char	*path;
	char	*cwd;

	if (!cmd->args || !cmd->args[1])
	{
		path = get_env_value(env_list, "HOME");
		if (!path)
			return (ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO), 1);
		else if (!path[0])
			return (0);
	}
	else
		path = cmd->args[1];
	if (chdir(path) == -1)
		return (ft_perror("cd"), 1);
	add_env_value(env_list, "OLDPWD", get_env_value(env_list, "1PWD"));
	cwd = ft_getcwd();
	if (cwd)
	{
		add_env_value(env_list, "1PWD", cwd);
		add_env_value(env_list, "PWD", cwd);
	}
	return (0);
}
