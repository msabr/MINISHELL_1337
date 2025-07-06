/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:51:05 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 20:39:42 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	cd(t_cmd *cmd, t_env **env_list)
{
	char	*path;
	char	*cwd;

	if (!cmd->args || !cmd->args[1])
	{
		path = get_env_value(env_list, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return ;
		}
		else if (!path[0])
			return ;
	}
	else
		path = cmd->args[1];
	if (chdir(path) == -1)
		return (perror("cd"));
	add_env_value(env_list, "OLDPWD", get_env_value(env_list, "1PWD"));
	cwd = getcwd(0, 0);
	if (cwd)
		add_env_value(env_list, "1PWD", cwd);
}
//cd: error retrieving current directory: getcwd: cannot 
//access parent directories: No such file or directory
