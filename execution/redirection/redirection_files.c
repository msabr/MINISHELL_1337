/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 12:48:33 by msabr            ###   ########.fr       */
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

static bool	valid_filename(const char *filename)
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

int	handel_heredoc(t_cmd *cmds, t_env *env)
{
	t_cmd		*current_cmd;
	t_redir	*curr;
	int		ret;
	
	current_cmd = cmds;
	while (current_cmd)
	{
		curr = current_cmd->redirs;
		while (curr)
		{
			if (curr->type == TOKEN_HEREDOC)
			{
				ret = redirect_heredoc(curr, env);
				if (ret != 0)
					return (ret);
			}
			curr = curr->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
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
