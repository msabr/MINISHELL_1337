/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_overwrite.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 00:42:28 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Function to handle output redirection using '>'
// void	redirect_overwrite(t_cmd *cmd)
// {
// 	int	fd;

// 	if (!cmd->redirs || !cmd->redirs->filename)
// 	{
// 		ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", STDERR_FILENO);
// 		return ;
// 	}
// 	fd = open(cmd->redirs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		perror("minishell");
// 		return ;
// 	}
// 	if (dup2(fd, STDOUT_FILENO) < 0)
// 	{
// 		perror("minishell");
// 		close(fd);
// 		return ;
// 	}
// 	close(fd);
// }

void redirect_overwrite(t_cmd *cmd)
{
    t_redir *redir = cmd->redirs;
    int fd = -1;


    while (redir)
    {
		
        if (redir->type == TOKEN_REDIR_OUT) // Your value for '>' redirection
        {
            // Close previous fd if we had multiple overwrites
            if (fd >= 0)
                close(fd);

            fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                fprintf(stderr, "Error opening file '%s': %s\n", redir->filename, strerror(errno));
                cmd->exit_status = 1;
                return;
            }
        }
        redir = redir->next;
    }
    // Only redirect if we found an overwrite redirection
    if (fd >= 0)
    {
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            fprintf(stderr, "dup2 error: %s\n", strerror(errno));
            close(fd);
            cmd->exit_status = 1;
            return;
        }
        close(fd);
    }
}

void redirect_overwrite_multy(t_cmd *cmd)
{
    t_redir *redir = cmd->redirs;
    int fd = -1;

    while (redir)
    {
        if (redir->type == TOKEN_REDIR_OUT)
        {
            // Close previous fd if there were multiple overwrites
            if (fd >= 0)
                close(fd);

            fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                fprintf(stderr, "Error opening file '%s': %s\n", redir->filename, strerror(errno));
                cmd->exit_status = 1;
                return;
            }
        }
        redir = redir->next;
    }
    if (fd >= 0)
    {
        if (dup2(fd, STDOUT_FILENO) < 0)
        {
            fprintf(stderr, "dup2 error: %s\n", strerror(errno));
            close(fd);
            cmd->exit_status = 1;
            return;
        }
        close(fd);
    }
}

