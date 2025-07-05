/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/05 17:18:13 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_redirection(t_cmd *cmds)
{
	t_redir		*current;
	t_heredoc	*current_heredoc;

	current = cmds->redirs;
	current_heredoc = cmds->heredocs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN)
			return (true);
		current = current->next;
	}
	while (current_heredoc)
	{
		if (current_heredoc->delimiter)
			return (true);
		current_heredoc = current_heredoc->next;
	}
	return (false);
}

static bool	handle_all_heredocs(t_cmd *cmds)
{
	t_heredoc	*current_heredoc;

	current_heredoc = cmds->heredocs;
	while (current_heredoc)
	{
		if (current_heredoc->delimiter)
		{
			handle_heredoc(cmds);
			if (cmds->exit_status != 0)
			{
				fprintf(stderr, "minishell: heredoc error\n");
				return (false);
			}
		}
		current_heredoc = current_heredoc->next;
	}
	return (true);
}

static bool	handle_all_file_redirs(t_cmd *cmds)
{
	t_redir	*current;
	int		flag;

	flag = 0;
	current = cmds->redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
			flag = redirect_stdin(current->filename);
		else if (current->type == TOKEN_REDIR_OUT)
			flag = redirect_overwrite(current->filename);
		else if (current->type == TOKEN_REDIR_APPEND)
			flag = redirect_append(current->filename);
		if (flag)
			return (false);
		current = current->next;
	}
	return (true);
}

bool	handle_redirections(t_cmd *cmds)
{
	if (!handle_all_heredocs(cmds))
		return (false);
	if (!handle_all_file_redirs(cmds))
		return (false);
	return (true);
}

// int execve_simple_cmd(t_cmd *cmds, t_env **env_list)
// {
// 	int		status;
// 	char	*path;
// 	pid_t	pid;
// 	status = 0;
// 	if (is_builtin(cmds->args[0]))
// 	{
// 		execve_builtin(cmds->args, env_list);
// 		return (0);
// 	}
// 	if (ft_strchr(cmds->args[0], '/'))
// 	{
// 		if (!ft_is_dir(cmds->args[0]))
// 			path = cmds->args[0];
// 		else
// 		{
// 			ft_putstr_fd("minishell: ", STDERR_FILENO);
// 			if (ft_is_dir(cmds->args[0]))
// 			{
// 				ft_putstr_fd(cmds->args[0], STDERR_FILENO);
// 				ft_putstr_fd(": is a directory\n", STDERR_FILENO);
// 			}
// 			else
// 			{
// 				access(cmds->args[0], X_OK);
// 				perror(cmds->args[0]);
// 			}
// 			return (1);
// 		}
// 	}
// 	else
// 	{
// 		path = get_path(cmds->args[0], *env_list);
// 		if (!path)
// 		{
// 			ft_putstr_fd("Command not found: ", STDERR_FILENO);
// 			ft_putstr_fd(cmds->args[0], STDERR_FILENO);
// 			ft_putchar_fd('\n', STDERR_FILENO);
// 			return (127);
// 		}
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 		perror("fork");
// 	else if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		signal(SIGQUIT, SIG_DFL);
// 		if (is_builtin(cmds->args[0]))
// 			execve_builtin(cmds->args, env_list);
// 		else
// 			execve(path, cmds->args, list_to_env(*env_list));
// 		perror("execve");
// 		exit(EXIT_FAILURE);
// 	}
// 	else
// 		status = handle_exit_status(pid);
// 	return (status);
// }