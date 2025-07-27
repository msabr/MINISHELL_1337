/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 13:01:53 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 19:40:22 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"

bool	is_heredoc(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
				return (true);
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	return (false);
}

void	check_nbr_heredocs(t_cmd *cmds)
{
	int		count;
	t_redir	*redir;

	count = 0;
	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
				count++;
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	if (count > MAX_HEREDOCS)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd("maximum here-document count exceeded\n", STDERR_FILENO);
		exit(2);
	}
}

bool	handel_heredoc(t_cmd *cmds, t_env *env)
{
	t_cmd	*current_cmd;
	t_redir	*curr;

	current_cmd = cmds;
	while (current_cmd)
	{
		curr = current_cmd->redirs;
		while (curr)
		{
			if (curr->type == TOKEN_HEREDOC)
				if (redirect_heredoc(curr, env))
					return (false);
			curr = curr->next;
		}
		current_cmd = current_cmd->next;
	}
	return (true);
}
