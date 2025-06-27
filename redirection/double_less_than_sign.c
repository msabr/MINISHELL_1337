/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_less_than_sign.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:58 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 17:26:17 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Function to handle input redirection using '<<'
void	heredoc(t_cmd *cmd)
{
    char *line;
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        line = readline("heredoc> ");
        if (!line || ft_strcmp(line, cmd->input_file) == 0)
        {
            free(line);
            break;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);
}