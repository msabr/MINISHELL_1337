/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:30 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 20:30:16 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"

char	*get_temp_filename(void)
{
	char	*file_name;
	int		n;

	file_name = ft_itoa((int)&n);
	file_name = ft_strjoin(file_name, "heredoc");
	return (file_name);
}

void	handle_heredoc_signal(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(STDOUT_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_redisplay();
		g_status = 1;
	}
}

int	redirect_heredoc(t_redir *redirs, t_env *env)
{
	char		*line;
	char		*tmp_file;
	char		*expanded;
	t_heredoc	*heredoc;

	heredoc = redirs->heredoc;
	tmp_file = get_temp_filename();
	if (!tmp_file)
		return (1);
	heredoc->fd_write = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	heredoc->fd_read = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	if (heredoc->fd_write < 0 || heredoc->fd_read < 0)
	{
		ft_perror(tmp_file);
		return (1);
	}
	signal(SIGINT, handle_heredoc_signal);
	while (1)
	{
		line = readline("> ");
		if (g_status == 1)
		{
			close(heredoc->fd_write);
			return (unlink(tmp_file), 1);
		}
		if (!line || ft_strcmp(line, heredoc->delimiter) == 0)
		{
			break ;
		}
		if (heredoc->flag == 0)
		{
			expanded = expand_heredoc_content(line, &env, g_status, heredoc->delimiter);
			if (expanded)
				ft_putstr_fd(expanded, heredoc->fd_write);
			else
				ft_putstr_fd(line, heredoc->fd_write);
		}
		else
			ft_putstr_fd(line, heredoc->fd_write);
		ft_putstr_fd("\n", heredoc->fd_write);
	}
	close(heredoc->fd_write);
	unlink(tmp_file);
	return (0);
}
