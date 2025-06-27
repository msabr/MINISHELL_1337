/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:13:27 by msabr             #+#    #+#             */
/*   Updated: 2025/06/25 20:29:10 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execve_cmd(char **args, t_env **env_list, char **env)
{
	int		status;
	char	*path;
	pid_t	pid;

	path = get_path(args[0], *env_list);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);

	}
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		if (is_bultins(args[0]))
			execve_builtin(args, env_list);
		else
			execve(path, args, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &status, 0);
}

void tt(void)
{
	write(1, "\033[32m----------------------\033[0m\n\n", 32);
}

char *get_path(char *cmd, t_env *env_list)
{
	char	*path_env;
	t_env	*current;
	// char	**paths;
	// char	*full_path;

	path_env = NULL;
	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->key, "PATH", 4) == 0)
		{
			path_env = current->value;
			break;
		}
		current = current->next;
	}
	if (!path_env)
		return cmd;
	char **paths = ft_split(path_env, ':');
	char *full_path = NULL;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return cmd;
		else
			return NULL;
	}
	for (int i = 0; paths[i]; i++)
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, X_OK | F_OK) == 0)
		{
			free_split(paths);
			return full_path;
		}
		free(full_path);
	}
	free_split(paths);
	return NULL;
}
