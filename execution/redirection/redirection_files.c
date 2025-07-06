/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 01:32:59 by msabr            ###   ########.fr       */
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
