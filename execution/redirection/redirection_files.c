/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 00:17:08 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool is_redirection(t_cmd *cmds)
{
    t_redir *current = cmds->redirs;
    while (current)
    {
        if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_REDIR_APPEND ||
            current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool handle_redirections(t_cmd *cmds)
{
    t_cmd *current = cmds;
    
    while (current)
    {
        if (current->redirs)
        {
            t_redir *redir = current->redirs;
            while (redir)
            {
                if (redir->type == TOKEN_REDIR_OUT)
                    redirect_overwrite(current);
                else if (redir->type == TOKEN_REDIR_APPEND)
                    redirect_append(current);
                else if (redir->type == TOKEN_REDIR_IN)
                    redirect_stdin(current);
                // else if (redir->type == TOKEN_HEREDOC)
                //     handle_heredoc(redir);
                redir = redir->next;
            }
        }
        current = current->next;
    }
    return true;
}


