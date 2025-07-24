/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:30 by msabr             #+#    #+#             */
/*   Updated: 2025/07/24 19:47:05 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handel_herdoc(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		signal(SIGINT, SIG_DFL);
		close(0);
	}
}

int	count_heredocs(t_cmd *cmds)
{
	int		count;
	t_redir	*redir;

	count = 0;
	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				count++;
				if (count > 16)
					return (-1);
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	return (count);
}
void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 2;
		write(1, "\n", 1); // Move to new line for prompt
		// Optionally: clean up temp files or memory here
		close(0); // Close stdin to break out of readline/get_next_line
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}
int	redirect_heredoc(t_cmd *cmd, t_env *env)
{
	t_redir *current = cmd->redirs;
	t_heredoc *heredoc;
	char *line;
	int fd;
	char tmp_file[32];
	int last_fd = -1;
	int heredoc_num = 0;
	struct sigaction sa;
	struct sigaction old_sa;
	
	sa.sa_handler = handle_heredoc_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	if (count_heredocs(cmd) < 0)
	{
		fprintf(stderr, "heredoc: maximum here-document count exceeded\n");
		g_status = 2;
		exit(2);
	}

	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
		{
			heredoc = current->heredoc;
			if (!heredoc || !heredoc->delimiter)
				return (1);

			snprintf(tmp_file, sizeof(tmp_file), "/tmp/minishell_heredoc_%d", heredoc_num++);
			fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd < 0)
				return (1);

			// Read input until delimiter is found
			while (1)
			{
				line = readline("> ");
				if (!line || ft_strcmp(line, heredoc->delimiter) == 0)
				{
					free(line);
					break;
				}
				if (heredoc->flag == 0)
				{
					char *expanded = expand_heredoc_content(line, &env, g_status, heredoc->delimiter);
					if (expanded)
					{
						ft_putstr_fd(expanded, fd);
						free(expanded);
					}
					else
						ft_putstr_fd(line, fd);
				}
				else
					ft_putstr_fd(line, fd);
				ft_putstr_fd("\n", fd);
				free(line); // <-- only free here, after done using line
			}
			close(fd);

			// Store the tmp filename for later use (e.g., in current->filename or a new field)
			// if (current->filename)
			// 	free(current->filename);
			current->filename = strdup(tmp_file);

			last_fd = heredoc_num - 1; // Track last heredoc index
		}
		current = current->next;
	}

sigaction(SIGINT, &old_sa, NULL);
	// Redirect stdin to the last heredoc's file
	if (last_fd != -1)
	{
		snprintf(tmp_file, sizeof(tmp_file), "/tmp/minishell_heredoc_%d", last_fd);
		fd = open(tmp_file, O_RDONLY);
		if (fd < 0)
			return (1);
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			return (1);
		}
		close(fd);
		unlink(tmp_file); // Optionally unlink here or after command finishes
	}

	return (0);
}