/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_stdin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:08:43 by msabr             #+#    #+#             */
/*   Updated: 2025/07/01 17:19:16 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Function to handle input redirection using '<'
void	redirect_stdin(t_cmd *cmd)
{
    int fd;

    if (cmd->redirs->filename)
    {
        fd = open(cmd->redirs->filename, O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }
        if (dup2(fd, STDIN_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
    }
    else
    {
        ft_putstr_fd("No input file specified for redirection\n", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
}
