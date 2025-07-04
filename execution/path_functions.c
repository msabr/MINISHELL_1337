/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:13:27 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 17:10:00 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_is_dir(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY | O_DIRECTORY);
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

static char	*find_path_env(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->key, "PATH", 4) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*search_in_paths(char *cmd, char **paths)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = get_fill_path(cmd, paths[i]);
		if (!ft_is_dir(full_path) && access(full_path, X_OK | F_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*result;

	path_env = find_path_env(env_list);
	if (!path_env)
		return (cmd);
	paths = ft_split(path_env, ':');
	result = search_in_paths(cmd, paths);
	return (result);
}
