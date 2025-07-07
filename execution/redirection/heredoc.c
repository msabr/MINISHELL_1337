/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:58 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 00:13:20 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_heredoc(t_cmd *cmd)
{
	int			pipe_fd[2];
	char		*line;
	t_heredoc	*current;

	line = NULL;
	current = cmd->heredocs;
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"));
	while (current)
	{
		while (true)
		{
			line = readline("> ");
			if (!line || !strcmp(line, current->delimiter))
				break ;
			ft_putendl_fd(line, pipe_fd[1]);
			free(line);
		}
		free(line);
		current = current->next;
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}
