/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:13:27 by msabr             #+#    #+#             */
/*   Updated: 2025/07/16 12:36:28 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	 is_directory(const char *path)
{
	int	fd;

	fd = open(path, O_DIRECTORY);
	if (fd < 0)
	{
		return (false);
	}
	close(fd);
	return (true);
}

static char	*get_fill_path(char *cmd, char *path_env)
{
	char	*full_path;

	full_path = ft_strjoin(path_env, "/");
	full_path = ft_strjoin(full_path, cmd);
	return (full_path);
}

static char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = get_fill_path(cmd, paths[i]);
		if (! is_directory(full_path) && access(full_path, F_OK) == 0)
		{
			if (access(full_path, X_OK) == 0)
				return (full_path);
			return (NULL);
		}
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*result;

	path_env = get_env_value(&env_list, "PATH");
	if (!path_env || path_env[0] == '\0')
		path_env = ":.";
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(cmd, paths);
	return (result);
}
