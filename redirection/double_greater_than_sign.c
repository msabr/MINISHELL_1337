/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_greater_than_sign.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:38 by msabr             #+#    #+#             */
/*   Updated: 2025/06/23 18:09:49 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to handle output redirection using '>>'
void	handle_double_output_redirection(t_cmd *cmd)
{
    if (cmd->output_file)
    {
        cmd->input_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (cmd->input_fd < 0)
        {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        if (dup2(cmd->input_fd, STDOUT_FILENO) < 0)
        {
            perror("Error redirecting output");
            exit(EXIT_FAILURE);
        }
        close(cmd->input_fd);
    }
}