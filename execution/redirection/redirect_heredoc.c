/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:30 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 04:05:41 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"

char	*ft_readline_heredoc(const char *prompt)
{
	char	*temp;
	char	*input;

	temp = readline(prompt);
	if (!temp)
		return (NULL);
	input = ft_strdup(temp);
	free(temp);
	return (input);
}

int	heredoc_file_setup(t_heredoc *heredoc)
{
	heredoc->tmp_file = get_temp_filename();
	if (!*heredoc->tmp_file)
		return (1);
	heredoc->fd_write = open(heredoc->tmp_file,
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
	heredoc->fd_read = open(heredoc->tmp_file, O_RDONLY);
	unlink(heredoc->tmp_file);
	if (heredoc->fd_write < 0 || heredoc->fd_read < 0)
		return (ft_perror(heredoc->tmp_file), 1);
	return (0);
}

void	heredoc_content_handler(t_heredoc *heredoc, char *line)
{
	char	*expanded;

	if (heredoc->flag == 0)
	{
		expanded = expand_heredoc_content(line, heredoc->env,
				heredoc->delimiter);
		if (expanded)
			ft_putstr_fd(expanded, heredoc->fd_write);
		else
			ft_putstr_fd(line, heredoc->fd_write);
	}
	else
		ft_putstr_fd(line, heredoc->fd_write);
	ft_putstr_fd("\n", heredoc->fd_write);
}

int	heredoc_signal_and_prompt(t_heredoc *heredoc)
{
	char	*line;

	signal(SIGINT, handle_heredoc_signal);
	while (1)
	{
		line = ft_readline_heredoc("> ");
		if (g_status == 1)
			return (1);
		if (!line || ft_strcmp(line, heredoc->delimiter) == 0)
			break ;
		heredoc_content_handler(heredoc, line);
	}
	return (0);
}

int	redirect_heredoc(t_redir *redirs, t_env *env)
{
	t_heredoc	*heredoc;
	int			result;

	heredoc = redirs->heredoc;
	heredoc->env = &env;
	result = heredoc_file_setup(heredoc);
	if (result)
		return (1);
	result = heredoc_signal_and_prompt(heredoc);
	return (result);
}
