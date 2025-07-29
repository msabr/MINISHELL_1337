/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:45:59 by msabr             #+#    #+#             */
/*   Updated: 2025/07/29 04:39:52 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
