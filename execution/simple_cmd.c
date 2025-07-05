/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:45:59 by msabr             #+#    #+#             */
/*   Updated: 2025/07/05 18:29:02 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_dir_error(char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (ft_is_dir(cmd))
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
	}
	else
	{
		access(cmd, X_OK);
		perror(cmd);
	}
	return (1);
}

int	get_exec_path(t_cmd *cmds, t_env **env_list, char **path)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (1);
	if (ft_strchr(cmds->args[0], '/'))
	{
		if (!ft_is_dir(cmds->args[0]))
		{
			*path = cmds->args[0];
			return (0);
		}
		else
			return (print_dir_error(cmds->args[0]));
	}
	else
	{
		*path = get_path(cmds->args[0], *env_list);
		if (!*path)
		{
			ft_putstr_fd("Command not found: ", STDERR_FILENO);
			ft_putstr_fd(cmds->args[0], STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			return (127);
		}
	}
	return (0);
}

void	exec_child_process(t_cmd *cmds, t_env **env_list, char *path)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (is_redirection(cmds))
	{
		if (!handle_redirections(cmds))
		{
			ft_putstr_fd("1Redirection error\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		if (cmds->exit_status != 0)
			exit(cmds->exit_status);
	}
	if (is_builtin(cmds->args[0]))
		execve_builtin(cmds->args, env_list);
	else
		execve(path, cmds->args, list_to_env(*env_list));
	perror("execve");
	exit(EXIT_FAILURE);
}

static int	handle_redir_and_builtin(t_cmd *cmds, t_env **env_list)
{
	if (is_redirection(cmds))
	{
		if (!handle_redirections(cmds))
			return (1);
		if (cmds->exit_status != 0)
			return (cmds->exit_status);
	}
	else if (is_builtin(cmds->args[0]))
	{
		execve_builtin(cmds->args, env_list);
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
