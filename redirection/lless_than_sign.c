/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lless_than_sign.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:08:43 by msabr             #+#    #+#             */
/*   Updated: 2025/06/23 18:08:54 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to handle input redirection using '<'
void	handle_input_redirection(t_cmd *cmd)
{
    if (cmd->input_file)
    {
        cmd->input_fd = open(cmd->input_file, O_RDONLY);
        if (cmd->input_fd < 0)
        {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
        if (dup2(cmd->input_fd, STDIN_FILENO) < 0)
        {
            perror("Error redirecting input");
            exit(EXIT_FAILURE);
        }
        close(cmd->input_fd);
    }
}