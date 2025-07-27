/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 13:23:59 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"

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

bool	valid_filename(const char *filename)
{
	if (!filename)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	handle_redirections(t_cmd *cmds)
{
	t_redir	*current;
	int		flag;

	flag = 0;
	current = cmds->redirs;
	while (current)
	{
		if (!valid_filename(current->filename))
			return (false);
		if (current->type == TOKEN_REDIR_IN)
			flag = redirect_stdin(current->filename);
		else if (current->type == TOKEN_REDIR_OUT)
			flag = redirect_overwrite(current->filename);
		else if (current->type == TOKEN_REDIR_APPEND)
			flag = redirect_append(current->filename);
		else if (current->type == TOKEN_HEREDOC)
		{
			dup2(current->heredoc->fd_read, STDIN_FILENO);
			close(current->heredoc->fd_read);
		}
		if (flag)
			return (false);
		current = current->next;
	}
	return (true);
}
