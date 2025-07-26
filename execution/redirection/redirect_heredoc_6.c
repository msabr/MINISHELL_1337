/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_6.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:10:56 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 17:14:47 by msabr            ###   ########.fr       */
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
int redirect_heredoc(t_cmd *cmd, t_env *env)
{
    t_redir *current = cmd->redirs;
    t_heredoc *heredoc;
    char *line;
    int fd;
    char *tmp_file = "/tmp/minishell_heredoc_tmp";
    
    if (count_heredocs(cmd) < 0)
	{
		fprintf(stderr, "heredoc: maximum here-document count exceeded\n");
		g_status = 2;
		exit(2);
	}
    while (current)
    {
		signal(SIGINT, handel_herdoc); // Handle Ctrl+C in heredoc
        if (current->type == TOKEN_HEREDOC)
        {
            heredoc = current->heredoc;
            if (!heredoc || !heredoc->delimiter)
                return (1); // Error: missing heredoc or delimiter

            // Open a temporary file for writing heredoc content
            fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0)
                return (1); // Error: failed to open temp file

            // Read input until delimiter is encountered
            while (1)
            {
                line = readline("> ");
                if (!line || ft_strcmp(line, heredoc->delimiter) == 0)
                {
                    free(line);
                    break;
                }

                // Expand variables if heredoc is not quoted
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
                free(line);
            }

            close(fd);

            // Open the temp file for reading and redirect to stdin
            fd = open(tmp_file, O_RDONLY);
            if (fd < 0)
                return (1); // Error: failed to open temp file for reading

            if (dup2(fd, STDIN_FILENO) < 0)
            {
                close(fd);
                return (1); // Error: failed to redirect
            }
            close(fd);
            unlink(tmp_file); // Remove temporary file
        }
        current = current->next;
    }
    return (0); // Success
}