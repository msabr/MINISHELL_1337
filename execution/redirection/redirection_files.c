/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 23:25:52 by msabr            ###   ########.fr       */
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
		ft_putstr_fd("heredoc: maximum here-document count exceeded\n", 2);
		exit(2);
	}
}

int	handel_heredoc(t_redir *redirs, t_env *env)
{
	int		ret;
	int		last_heredoc_fd;
	t_redir	*curr;

	curr = redirs;
	last_heredoc_fd = -1;
	while (curr)
	{
		if (curr->type == TOKEN_HEREDOC)
		{
			ret = redirect_heredoc(curr, env);
			if (ret != 0)
				return (1);
			last_heredoc_fd = curr->heredoc->fd_read;
		}
		curr = curr->next;
	}
	if (last_heredoc_fd != -1)
		dup2(last_heredoc_fd, STDIN_FILENO);
	return (0);
}

bool	handle_redirections(t_cmd *cmds, t_env *env)
{
	t_redir	*current;
	int		flag;

	flag = 0;
	check_nbr_heredocs(cmds);
	current = cmds->redirs;
	if (handel_heredoc(current, env))
		return (false);
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
		if (flag)
			return (false);
		current = current->next;
	}
	return (true);
}
