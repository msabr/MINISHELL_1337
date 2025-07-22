/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 17:45:00 by khalid058r        #+#    #+#             */
/*   Updated: 2025/07/22 18:02:34 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Handler for SIGINT in heredoc */
void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130; // convention bash pour Ctrl+C
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
		free(*line);
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
	free(line);
	if (!l_dup)
		return (close(fds[0]), close(fds[1]), -1);

	if (ft_strcmp(l_dup, heredoc->delimiter) == 0)
		return (free(l_dup), 1); // fin heredoc

	if (heredoc->flag == 0) // Non quoted: expansion
		expand_heredoc_line(&l_dup, heredoc->env);

	write(fds[1], l_dup, ft_strlen(l_dup));
	write(fds[1], "\n", 1);
	free(l_dup);
	return (0);
}

/* Ouvre le heredoc, renvoie le fd à utiliser comme STDIN */
int	heredoc_pipe(const char *delim, t_env **env, int quoted)
{
	int			fds[2];
	int			status;
	t_heredoc	heredoc;

	heredoc.delimiter = (char *)delim;
	heredoc.env = env;
	heredoc.flag = quoted;
	heredoc.fd_read = -1;
	heredoc.fd_write = -1;

	if (pipe(fds) < 0)
		return (ft_perror("pipe"), -1);

	signal(SIGINT, heredoc_sigint_handler);

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
int	count_heredocs(t_cmd *cmds)
{
	int		count = 0;
	t_redir	*redir;

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
	return count;
}

/* Prépare chaque heredoc et assigne le fd dans la redirection */
int	handle_heredoc(t_redir *redir, t_env **env)
{
	int	fd;

	while (redir)
	{
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

/* À appeler avant exec pour ouvrir tous les heredocs */
int	preprocess_heredocs(t_cmd *cmds, t_env **env)
{
	if (count_heredocs(cmds) < 0)
	{
		fprintf(stderr, "heredoc: maximum here-document count exceeded\n");
		g_status = 2;
		exit(2);
	}
	while (cmds)
	{
		if (handle_heredoc(cmds->redirs, env) == -1)
			return (-1);
		cmds = cmds->next;
	}
	return (0);
}