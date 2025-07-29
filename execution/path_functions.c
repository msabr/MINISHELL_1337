/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:13:27 by msabr             #+#    #+#             */
/*   Updated: 2025/07/29 05:13:44 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static bool	ft_aid(char *cmd, t_env **env_list)
{
	int		i;
	char	**paths;
	char	*path_env;
	char	*full_path;

	i = 0;
	path_env = get_env_value(env_list, "PATH");
	if (!path_env || path_env[0] == '\0')
		path_env = ":.";
	paths = ft_split(path_env, ':');
	if (!paths)
		return (false);
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], ft_strjoin("/", cmd));
		if (!is_directory(full_path) && access(full_path, F_OK) == 0)
			return (true);
		i++;
	}
	return (false);
}

static int	handle_path_and_errors(t_cmd *cmds, char **path)
{
	if (is_directory(cmds->args[0]))
		return (print_dir_error(cmds->args[0]));
	if (access(cmds->args[0], X_OK) == -1)
	{
		ft_perror(cmds->args[0]);
		if (errno == ENOENT)
			return (127);
		return (126);
	}
	*path = ft_strdup(cmds->args[0]);
	if (!*path)
	{
		ft_perror(cmds->args[0]);
		return (1);
	}
	return (0);
}

int	get_exec_path(t_cmd *cmds, t_env **env_list, char **path)
{
	char	*tmp;

	if (!cmds || !cmds->args || !cmds->args[0])
		return (1);
	if (ft_strchr(cmds->args[0], '/')
		|| !*get_env_value(env_list, "PATH"))
		return (handle_path_and_errors(cmds, path));
	tmp = get_path(cmds->args[0], *env_list);
	if (!tmp && ft_aid(cmds->args[0], env_list))
	{
		access(cmds->args[0], X_OK);
		ft_perror(cmds->args[0]);
		return (126);
	}
	if (!tmp)
		return (print_cmd_not_found_error(cmds->args[0]));
	*path = tmp;
	return (0);
}
