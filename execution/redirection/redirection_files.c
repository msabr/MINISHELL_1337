/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/07/02 17:31:37 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool is_redirection(char *cmd)
{
    return (strchr(cmd, LLESS_THAN_SIGNESS) || strchr(cmd, GREAGREATER_THAN_SIGN) ||
            strstr(cmd, DOUBLE_GREATER_THAN_SIGN) || strstr(cmd, DOUBLE_LESS_THAN_SIGN));
}

void handle_redirections(t_cmd *cmds)
{
    t_redir *current = cmds->redirs;
    while (current)
    {
        // if (current->type == TOKEN_HEREDOC)
        // {
        //     heredoc(cmds); // Handle heredoc redirection
        // }
        if (current->type == TOKEN_REDIR_OUT)
        {
            redirect_overwrite(cmds); // Handle overwrite redirection
        }
        else if (current->type == TOKEN_REDIR_APPEND)
        {
            redirect_append(cmds); // Handle append redirection
        }
        else if (current->type == TOKEN_REDIR_IN)
        {
            redirect_stdin(cmds); // Handle input redirection
        }
        else
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
            ft_putstr_fd(current->filename, STDERR_FILENO);
            ft_putstr_fd("'\n", STDERR_FILENO);
        }
        current = current->next;
    }
}


