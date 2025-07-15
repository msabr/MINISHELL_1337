/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/14 19:44:02 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_redirection(t_cmd *cmds)
{
	t_redir		*current;

	current = cmds->redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_HEREDOC)
			return (true);
		current = current->next;
	}
	return (false);
}

bool	handle_redirections(t_cmd *cmds, t_env *env)
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
		else if (current->type == TOKEN_HEREDOC)
			flag = redirect_heredoc(cmds, env);
		if (flag)
			return (false);
		current = current->next;
	}
	return (true);
}

void	save_std_fds(t_cmd *cmds)
{
	if (cmds->redirs)
	{
		cmds->redirs->fd_in = dup(STDIN_FILENO);
		cmds->redirs->fd_out = dup(STDOUT_FILENO);
	}
}

void	restore_std_fds(t_cmd *cmds)
{
	if (cmds->redirs)
	{
		dup2(cmds->redirs->fd_in, STDIN_FILENO);
		dup2(cmds->redirs->fd_out, STDOUT_FILENO);
		close(cmds->redirs->fd_in);
		close(cmds->redirs->fd_out);
	}
}
