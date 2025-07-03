/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_append.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:38 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 00:24:21 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Function to handle output redirection using '>>'
// void	redirect_append(t_cmd *cmd)
// {
// 	int fd;

// 	if (cmd->redirs->filename)
	
// 	{
// 		fd = open(cmd->redirs->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (fd < 0)
// 		{
// 			perror("open");
// 			exit(EXIT_FAILURE);
// 		}
// 		if (dup2(fd, STDOUT_FILENO) < 0)
// 		{
// 			perror("dup2");
// 			close(fd);
// 			exit(EXIT_FAILURE);
// 		}
// 		close(fd);
// 	}
// 	else
// 	{
// 		ft_putstr_fd("No output file specified for append redirection\n", STDERR_FILENO);
// 		exit(EXIT_FAILURE);
// 	}
// }
void redirect_append(t_cmd *cmd)
{
    t_redir *redir = cmd->redirs;
    int fd;
    int saved_stdout;

    while (redir)
    {
        if (redir->type == TOKEN_REDIR_APPEND)
        {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                fprintf(stderr, "Error opening file '%s': %s\n", redir->filename, strerror(errno));
                cmd->exit_status = 1;
                return;
            }
            saved_stdout = dup(STDOUT_FILENO);
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                fprintf(stderr, "dup2 error: %s\n", strerror(errno));
                close(fd);
                cmd->exit_status = 1;
                return;
            }
			dup2(saved_stdout, STDOUT_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
}

void redirect_append_mlty(t_cmd *cmd)
{
    t_redir *redir = cmd->redirs;
    int fd = -1;

    while (redir)
    {
        if (redir->type == TOKEN_REDIR_APPEND)
        {
            // Close previous fd if open
            if (fd >= 0)
                close(fd);

            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                fprintf(stderr, "Error opening file '%s': %s\n", redir->filename, strerror(errno));
                cmd->exit_status = 1;
                return;
            }
        }
        redir = redir->next;
    }
    // If we found any append redirection, do dup2
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