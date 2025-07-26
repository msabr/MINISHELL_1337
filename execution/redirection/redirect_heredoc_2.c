/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:32 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 21:56:58 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handel_herdoc(int sig)
{
	if (sig == SIGINT)
	{
        ioctl(STDOUT_FILENO, TIOCSTI, "\n"); // Simulate a newline in the terminal
		g_status = 130; // Set status to 130 for SIGINT
        rl_replace_line("", 0); // Clear the current line in readline
        rl_redisplay(); // Redisplay the prompt
		close(0);
	}
}

/* Expansion: $VAR, $?, $1, etc. (à adapter si tu veux plus précis) */
static void	expand_heredoc_line(char **line, t_env **env)
{
	char	*expanded;

	expanded = expand_heredoc_content(*line, env, g_status, NULL);
	if (expanded)
	{
		*line = expanded;
	}
}

/* Lit et traite une ligne du heredoc */
static int	handle_heredoc_line(int *fds, t_heredoc *heredoc)
{
	char	*line;
	char	*l_dup;

	line = readline("> ");
	if (!line)
		return (-1);
	l_dup = ft_strdup(line);
	if (!l_dup)
		return (close(fds[0]), close(fds[1]), -1);

	if (ft_strcmp(l_dup, heredoc->delimiter) == 0)
		return (1); // fin heredoc

	if (heredoc->flag == 0) // Non quoted: expansion
		expand_heredoc_line(&l_dup, heredoc->env);

	write(fds[1], l_dup, ft_strlen(l_dup));
	write(fds[1], "\n", 1);
	return (0);
}

/* Ouvre le heredoc, renvoie le fd à utiliser comme STDIN */
int	heredoc_pipe(const char *delim, t_env *env, int quoted)
{
	int			fds[2];
	int			status;
	t_heredoc	heredoc;

	heredoc.delimiter = (char *)delim;
	heredoc.env = &env;
	heredoc.flag = quoted;
	heredoc.fd_read = -1;
	heredoc.fd_write = -1;

	if (pipe(fds) < 0)
		return (ft_perror("pipe"), -1);

	// signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		status = handle_heredoc_line(fds, &heredoc);
		if (status != 0)
			break ;
	}

	close(fds[1]);
	signal(SIGINT, SIG_DFL); // Restaure le handler par défaut
	return fds[0];
}

/* Compte les heredocs de toutes les commandes, limite à 16 */


/* Prépare chaque heredoc et assigne le fd dans la redirection */
int	redirect_heredoc(t_redir *redir, t_env *env)
{
	int	fd;

	while (redir)
	{
		signal(SIGINT, handel_herdoc);
		if (redir->type == TOKEN_HEREDOC && redir->heredoc)
		{
			fd = heredoc_pipe(redir->heredoc->delimiter, env, redir->heredoc->flag);
			if (fd == -1)
				return (-1);
			redir->fd_in = fd;
		}
		redir = redir->next;
	}
	return (0);
}

