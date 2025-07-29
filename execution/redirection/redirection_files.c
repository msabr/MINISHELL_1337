/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/29 14:54:35 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"

void	close_heredoc(t_cmd *cmds)
{
	t_cmd		*current_cmd;
	t_redir		*current_redir;

	current_cmd = cmds;
	while (current_cmd)
	{
		current_redir = current_cmd->redirs;
		while (current_redir)
		{
			if (current_redir->type == TOKEN_HEREDOC)
			{
				if (current_redir->heredoc)
				{
					close(current_redir->heredoc->fd_read);
					close(current_redir->heredoc->fd_write);
				}
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
}

bool	is_directory(const char *path)
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

bool	handle_redirections(t_cmd *cmds)
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
		{
			if (dup2(current->heredoc->fd_read, STDIN_FILENO) < 0)
				return (false);
		}
		if (flag)
			return (false);
		current = current->next;
	}
	return (true);
}
