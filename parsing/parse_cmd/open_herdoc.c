#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

/*
** SIGINT handler for heredoc
*/
static void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_status = 130;
	write(1, "\n", 1);
	close(0);
}

/*
** Expansion de la ligne du heredoc si flag == 0 (non quoted)
*/
static void	expand_heredoc_line(char **line, t_env **env)
{
	char	*expanded;

	if (!line || !*line)
		return;
	// Appelle la fonction d'expansion déjà présente dans le projet
	expanded = expand_heredoc_content(*line, env, g_status, NULL);
	if (expanded)
	{
		free(*line);
		*line = expanded;
	}
}

/*
** Ouvre un tempfile sécurisé dans /tmp, renvoie son fd et son nom via *filename
*/
static int	open_tempfile(char **filename)
{
	char template[] = "/tmp/minishell_heredoc_XXXXXX";
	int fd;

	*filename = strdup(template);
	if (!*filename)
		return (-1);
	fd = mkstemp(*filename);
	if (fd == -1)
	{
		perror("mkstemp");
		free(*filename);
		*filename = NULL;
		return (-1);
	}
	return fd;
}

/*
** Ouvre, écrit et prépare le heredoc pour une redirection
*/
int	open_heredoc_tempfile(t_heredoc *heredoc)
{
	char		*tmpfile = NULL;
	int			fd_write, fd_read;
	char		*line;

	fd_write = open_tempfile(&tmpfile);
	if (fd_write < 0)
		return (-1);
	signal(SIGINT, handle_heredoc_sigint);
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
		if (heredoc->flag == 0)
			expand_heredoc_line(&line, heredoc->env);
		write(fd_write, line, strlen(line));
		write(fd_write, "\n", 1);
		free(line);
	}
	close(fd_write);
	signal(SIGINT, SIG_DFL);

	// Ouvre en lecture, supprime le fichier tout de suite (unlinked)
	fd_read = open(tmpfile, O_RDONLY);
	if (fd_read < 0)
	{
		perror("heredoc: open tempfile for reading");
		unlink(tmpfile);
		free(tmpfile);
		return (-1);
	}
	unlink(tmpfile);
	free(tmpfile);
	heredoc->fd_read = fd_read;
	return (0);
}

/*
** Parcourt toutes les redirections HEREDOC d'une liste de commandes,
** ouvre le heredoc, place le fd dans redir->fd_in et cmd->fd
*/
int	open_all_heredocs_tempfile(t_cmd *cmds)
{
	t_cmd   *current = cmds;
	while (current)
	{
		t_redir *redir = current->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC && redir->heredoc)
			{
				if (open_heredoc_tempfile(redir->heredoc) < 0)
					return (-1);
				redir->fd_in = redir->heredoc->fd_read;
				current->fd = redir->heredoc->fd_read;
			}
			redir = redir->next;
		}
		current = current->next;
	}
	return (0);
}