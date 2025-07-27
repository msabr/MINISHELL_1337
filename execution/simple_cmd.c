/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:45:59 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 13:05:14 by msabr            ###   ########.fr       */
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
		if (! is_directory(full_path) && access(full_path, F_OK) == 0)
			return (true);
		i++;
	}
	return (false);
}

int	get_exec_path(t_cmd *cmds, t_env **env_list, char **path)
{
	char	*tmp;

	if (!cmds || !cmds->args || !cmds->args[0])
		return (1);
	if (ft_strchr(cmds->args[0], '/') != NULL
		|| ft_strlen(get_env_value(env_list, "PATH")) == 0)
	{
		if (is_directory(cmds->args[0]))
			return (print_dir_error(cmds->args[0]));
		if (access(cmds->args[0], X_OK) == -1)
		{
			if (errno == ENOENT)
				return (ft_perror(cmds->args[0]), 127);
			return (ft_perror(cmds->args[0]), 126);
		}
		*path = ft_strdup(cmds->args[0]);
		if (!*path)
			return (ft_perror(cmds->args[0]), 1);
		return (0);
	}
	tmp = get_path(cmds->args[0], *env_list);
	if (!tmp && ft_aid(cmds->args[0], env_list))
		return (access(cmds->args[0], X_OK), ft_perror(cmds->args[0]), 126);
	else if (!tmp)
		return (print_cmd_not_found_error(cmds->args[0]));
	return (*path = tmp, 0);
}

static void	exec_child_process(t_cmd *cmds, t_env **env_list, char *path)
{
	set_default_signals();
	if (!cmds || !cmds->args || !cmds->args[0])
		ft_exit(EXIT_FAILURE);
	execve(path, cmds->args, list_to_env(*env_list));
	if (!path || !*path)
		ft_perror(cmds->args[0]);
	ft_exit(EXIT_FAILURE);
}

static int	handle_redir_and_builtin(t_cmd *cmds, t_env **env_list)
{
	if (!cmds)
		return (1);
	if (is_redirection(cmds))
	{
		if (!handle_redirections(cmds))
			return (1);
	}
	if (!cmds->args || !cmds->args[0])
		return (1);
	if (is_builtin(cmds->args[0]))
		return (execve_builtin(cmds, env_list));
	return (-1);
}

int	execve_simple_cmd(t_cmd *cmds, t_env **env_list)
{
	int		status;
	char	*path;
	pid_t	pid;

	path = NULL;
	status = handle_redir_and_builtin(cmds, env_list);
	if (status != -1)
		return (status);
	status = get_exec_path(cmds, env_list, &path);
	if (status != 0)
		return (status);
	pid = fork();
	if (pid < 0)
		return (ft_perror("fork"), 1);
	else if (pid == 0)
		exec_child_process(cmds, env_list, path);
	return (handle_exit_status(pid, status));
}
