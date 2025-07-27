/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*    file_descriptor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 22:49:19 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 13:14:16 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
