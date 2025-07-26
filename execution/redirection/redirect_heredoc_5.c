/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_5.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:30 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 22:33:29 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_temp_filename(void)
{
	static int	counter;
	char		*filename;
	char		*counter_str;

	while (1)
	{
		counter_str = ft_itoa(counter);
		if (!counter_str)
			return (NULL);
		filename = ft_strjoin("/tmp/herdoc", counter_str);
		if (!filename)
			return (NULL);
		if (access(filename, F_OK) != 0)
			return (filename);
		counter++;
	}
	return (NULL);
}

void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
        ioctl(STDOUT_FILENO, TIOCSTI, "\n"); // Simulate a newline in the terminal
		g_status = 130; // Set status to 130 for SIGINT
        rl_replace_line("", 0); // Clear the current line in readline
        rl_redisplay(); // Redisplay the prompt
	}
}
int	redirect_heredoc(t_redir *redirs, t_env *env)
{
	char		*line;
	char		*tmp_file = get_temp_filename();
	t_heredoc	*heredoc = redirs->heredoc;

	if (!heredoc || !heredoc->delimiter || !tmp_file)
		return (1);

	heredoc->fd_write = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (heredoc->fd_write < 0)
	{
		ft_perror(tmp_file);
		// free(tmp_file);
		return (1);
	}


	while (1)
	{
	signal(SIGINT, handle_heredoc_signal);
		line = readline("> ");
		if (g_status == 130)
		{
			free(line);
			close(heredoc->fd_write);
			close(heredoc->fd_read);
			unlink(tmp_file);
			// free(tmp_file);
			return (1);
		}
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
				ft_putstr_fd(expanded, heredoc->fd_write);
				// free(expanded);
			}
			else
				ft_putstr_fd(line, heredoc->fd_write);
		}
		else
			ft_putstr_fd(line, heredoc->fd_write);
		ft_putstr_fd("\n", heredoc->fd_write);
		// free(line);
	}
	close(heredoc->fd_write);
	// free(tmp_file);
	heredoc->fd_read = open(tmp_file, O_RDONLY);
	if (heredoc->fd_read < 0)
	{
		ft_perror(tmp_file);
		unlink(tmp_file);
		close(heredoc->fd_write);
		// free(tmp_file);
		return (1);
	}

	// if (dup2(heredoc->fd_read, STDIN_FILENO) < 0)
	// {
	// 	close(heredoc->fd_read);
	// 	return (1);
	// }
	close(heredoc->fd_read);
	signal(SIGINT, SIG_DFL); // Restore default signal handling
	return (0);
}