/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_overwrite.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:11 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 18:43:25 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Function to handle output redirection using '>'
void	redirect_overwrite(t_cmd *cmd)
{
	if (cmd->output_file)
	{
		cmd->output_fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->output_fd < 0)
		{
			perror("Error opening output file");
			exit(EXIT_FAILURE);
		}
		if (dup2(cmd->output_fd, STDOUT_FILENO) < 0)
		{
			perror("Error redirecting output");
			exit(EXIT_FAILURE);
		}
		close(cmd->output_fd);
	}
}

