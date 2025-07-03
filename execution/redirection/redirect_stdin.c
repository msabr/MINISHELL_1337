/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:08:43 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 00:28:25 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Function to handle input redirection using '<'
// void	redirect_stdin(t_cmd *cmd)
// {
//     int fd;

//     if (cmd->redirs->filename)
//     {
//         fd = open(cmd->redirs->filename, O_RDONLY);
//         if (fd < 0)
//         {
//             perror("open");
//             exit(EXIT_FAILURE);
//         }
//         if (dup2(fd, STDIN_FILENO) < 0)
//         {
//             perror("dup2");
//             close(fd);
//             exit(EXIT_FAILURE);
//         }
//         close(fd);
//     }
//     else
//     {
//         ft_putstr_fd("No input file specified for redirection\n", STDERR_FILENO);
//         exit(EXIT_FAILURE);
//     }
// }

void redirect_stdin(t_cmd *cmd)
{
    t_redir *redir = cmd->redirs;
    int fd = -1;

    while (redir)
    {
        if (redir->type == TOKEN_REDIR_IN) // Replace with the correct value for '<'
        {
            // Close previous fd if there are multiple input redirections
            if (fd >= 0)
                close(fd);

            fd = open(redir->filename, O_RDONLY);
            if (fd < 0)
            {
                fprintf(stderr, "Error opening file '%s' for input: %s\n", redir->filename, strerror(errno));
                cmd->exit_status = 1;
                return;
            }
        }
        redir = redir->next;
    }
    // Only redirect if we found an input redirection
    if (fd >= 0)
    {
        if (dup2(fd, STDIN_FILENO) < 0)
        {
            fprintf(stderr, "dup2 error: %s\n", strerror(errno));
            close(fd);
            cmd->exit_status = 1;
            return;
        }
        close(fd);
    }
}

void redirect_stdin_multy(t_cmd *cmd)
{
    t_redir *redir = cmd->redirs;
    int fd = -1;

    while (redir)
    {
        if (redir->type == TOKEN_REDIR_IN) // Replace with your enum value for '<'
        {
            // Close the previous fd if we had multiple input redirections
            if (fd >= 0)
                close(fd);

            fd = open(redir->filename, O_RDONLY);
            if (fd < 0)
            {
                fprintf(stderr, "Error opening file '%s' for input: %s\n", redir->filename, strerror(errno));
                cmd->exit_status = 1;
                return;
            }
        }
        redir = redir->next;
    }
    // Only redirect if at least one input redirection was found
    if (fd >= 0)
    {
        if (dup2(fd, STDIN_FILENO) < 0)
        {
            fprintf(stderr, "dup2 error: %s\n", strerror(errno));
            close(fd);
            cmd->exit_status = 1;
            return;
        }
        close(fd);
    }
}
