/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:45:59 by msabr             #+#    #+#             */
/*   Updated: 2025/07/12 18:17:46 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_exec_path(t_cmd *cmds, t_env **env_list, char **path)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (1);
	if (ft_strchr(cmds->args[0], '/'))
	{
		if (ft_is_dir(cmds->args[0]))
			return (print_dir_error(cmds->args[0]));
		if (access(cmds->args[0], X_OK | F_OK) == -1)
		{
			if (errno == ENOENT)
				return (print_execve_error(cmds->args[0]));
			else if (errno == EACCES)
				return (print_execve_permission_error(cmds->args[0]));
			else
				return (perror("minishell"), 126);
		}
		*path = ft_strdup(cmds->args[0]);
	}
	else
	{
		*path = get_path(cmds->args[0], *env_list);
		if (!*path)
			return (print_cmd_not_found_error(cmds->args[0]));
	}
	return (0);
}

void	exec_child_process(t_cmd *cmds, t_env **env_list, char *path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmds || !cmds->args || !cmds->args[0])
		exit(EXIT_FAILURE);
	if (is_redirection(cmds))
	{
		if (!handle_redirections(cmds))
		{
			ft_putstr_fd("Redirection error\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
	}
	if (is_builtin(cmds->args[0]))
		execve_builtin(cmds, env_list);
	else
		execve(path, cmds->args, list_to_env(*env_list));
	if (!path || !*path)
		perror("minishell");
	exit(EXIT_FAILURE);
}

static int	handle_redir_and_builtin(t_cmd *cmds, t_env **env_list)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (1);
	if (is_redirection(cmds))
	{
		if (!handle_redirections(cmds))
			return (1);
	}
	if (is_builtin(cmds->args[0]))
	{
		execve_builtin(cmds, env_list);
		return (0);
	}
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
		return (perror("fork"), 1);
	else if (pid == 0)
		exec_child_process(cmds, env_list, path);
	return (handle_exit_status(pid));
}
