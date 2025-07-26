/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_7.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:44:13 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 22:32:07 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
int redirect_heredoc(t_redir *redir, t_env *env)
{
    t_heredoc *heredoc = redir->heredoc;
    char *tmp_file = get_temp_filename();
    int write_fd, read_fd;
    char *line = NULL;

    if (!heredoc || !heredoc->delimiter || !tmp_file)
        return (1);

    write_fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (write_fd < 0) {
        ft_perror(tmp_file);
        free(tmp_file);
        return (1);
    }

    // heredoc_interrupted = 0;
    signal(SIGINT, heredoc_sigint_handler);
    while (1) 
	{
        line = readline("> ");
        if (!line)
            break;
        if (strcmp(line, heredoc->delimiter) == 0) 
		{
            free(line);
            break;
        }
        if (!heredoc->flag) {
            char *expanded = expand_heredoc_content(line, heredoc->env, g_status, heredoc->delimiter);
            if (expanded) {
                ft_putstr_fd(expanded, write_fd);
                free(expanded);
            } else {
                ft_putstr_fd(line, write_fd);
            }
        } else {
            ft_putstr_fd(line, write_fd);
        }
        ft_putstr_fd("\n", write_fd);
        free(line);
    }

    close(write_fd);

    read_fd = open(tmp_file, O_RDONLY);
    if (read_fd < 0) {
        ft_perror(tmp_file);
        unlink(tmp_file);
        free(tmp_file);
        return (1);
    }

    // Save fd and tmp_file for later use (setup redirection and cleanup)
    heredoc->fd_read = read_fd;
(void)env; 
    return (0);
}
