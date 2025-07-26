/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 22:21:47 by msabr            ###   ########.fr       */
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

static bool valid_filename(const char *filename)
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
int	count_heredocs(t_cmd *cmds)
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
	return (count);
}
int handel_heredoc(t_redir *redirs, t_env *env)
{
    t_redir *curr = redirs;
    int last_heredoc_fd = -1;

    while (curr)
    {
        if (curr->type == TOKEN_HEREDOC)
        {
            // Prompt and write heredoc content to temp file
            int ret = redirect_heredoc(curr, env);
            if (ret != 0)
            {
                // Cleanup all temp files created so far!
                return 1;
            }
            last_heredoc_fd = curr->heredoc->fd_read; // Save last fd
        }
        curr = curr->next;
    }
    // Only dup2 the last heredoc temp file as stdin
    if (last_heredoc_fd != -1)
    {
        dup2(last_heredoc_fd, STDIN_FILENO);
        // do not close here, but after exec!
    }
    return 0;
}

bool	handle_redirections(t_cmd *cmds, t_env *env)
{
	t_redir	*current;
	int		flag;

	flag = 0;
	if (count_heredocs(cmds) > MAX_HEREDOCS)
	{
		ft_putstr_fd("heredoc: maximum here-document count exceeded\n", STDERR_FILENO);
		exit(2);
	}
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
