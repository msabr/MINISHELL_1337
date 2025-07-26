/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:30 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 16:04:26 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		write(1, "\n", 1);
		close(STDIN_FILENO); // Close stdin so readline returns NULL
	}
}

int	redirect_heredoc(t_cmd *cmd, t_env *env_list)
{
	t_redir *current = cmd->redirs;
	t_heredoc *heredoc;
	char *line;
	int pipe_fd[2];
	pid_t pid;
	int status;
	int last_fd = -1;
	int eof_encountered = 0;
	int exit_code;

	while (current && !eof_encountered)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			if (pipe(pipe_fd) < 0)
				return (1);

			signal(SIGINT, SIG_IGN); // Ignore SIGINT in parent during heredoc
			pid = fork();
			if (pid < 0)
			{
				close(pipe_fd[0]);
				close(pipe_fd[1]);
				return (1);
			}
			if (pid == 0) // Child process
			{
				heredoc = current->heredoc;
				if (!heredoc || !heredoc->delimiter)
					return (1);
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_IGN);
				close(pipe_fd[0]);

				while (1)
				{
					line = readline("> ");
					if (!line) // EOF (^D)
					{
						close(pipe_fd[1]);
						exit(42); // Special exit code to indicate EOF
					}
					if (strcmp(line, heredoc->delimiter) == 0)
					{
						free(line);
						exit_code = 42;
						break; // Exit loop on delimiter match
					}
					if (heredoc->flag == 0)
					{
						char *expanded = expand_heredoc_content(line, &env_list, g_status, heredoc->delimiter);
						if (expanded)
						{
							ft_putstr_fd(expanded, pipe_fd[1]);
							free(expanded);
						}
					}
					else
					{
						ft_putstr_fd(line, pipe_fd[1]);
					}
					ft_putstr_fd("\n", pipe_fd[1]);
					free(line);
				}
				close(pipe_fd[1]);
				exit(0);
			}
			// Parent process
			close(pipe_fd[1]);
			waitpid(pid, &status, 0);
			signal(SIGINT, SIG_DFL);

			if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				if (sig == SIGINT)
				{
					ft_putstr_fd("\n", STDERR_FILENO);
					g_status = 130;
				}
				else
				{
					g_status = 128 + sig;
				}
				close(pipe_fd[0]);
				if (last_fd != -1)
					close(last_fd);
				return (1);
			}
			else if (WIFEXITED(status))
			{
				if (exit_code != 42)
					exit_code = WEXITSTATUS(status);
				if (exit_code == 42) // EOF detected
				{
					eof_encountered = 1;
					close(pipe_fd[0]);
					break;
				}
				else if (exit_code != 0)
				{
					close(pipe_fd[0]);
					if (last_fd != -1)
						close(last_fd);
					return (1);
				}
			}

			if (last_fd != -1)
				close(last_fd);
			last_fd = pipe_fd[0];
		}
		current = current->next;
	}

	if (last_fd != -1 && !eof_encountered)
	{
		dup2(last_fd, STDIN_FILENO);
		close(last_fd);
	}
	else if (last_fd != -1)
	{
		close(last_fd);
	}
	return (0);
}