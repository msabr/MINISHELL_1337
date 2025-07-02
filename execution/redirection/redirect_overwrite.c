/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_overwrite.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/01 17:18:54 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Function to handle output redirection using '>'
void	redirect_overwrite(t_cmd *cmd)
{
	int fd;

	if (cmd->redirs->filename)
	{
		fd = open(cmd->redirs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
	else
	{
		ft_putstr_fd("No output file specified for overwrite redirection\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

