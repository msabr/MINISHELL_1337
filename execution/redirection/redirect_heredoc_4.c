/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 15:41:45 by msabr             #+#    #+#             */
/*   Updated: 2025/07/26 22:03:44 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
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

int redirect_heredoc(t_redir *redirs, t_env *env)
{
    t_heredoc *heredoc = redirs->heredoc;
    char *line;
    int fd;
    char *tmp_file = "/tmp/minishell_heredoc_tmp";
    
		signal(SIGINT, handle_heredoc_signal); // Set signal handler for SIGINT


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
    return (0); // Success
}
// int	redirect_heredoc(t_cmd *cmd, t_env *env_list)
// {
//     int fd_read;
//     int fd_write;

//     fd_write = open("/tmp/heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     if (fd_write < 0)
//     {
//         tt();
//         ft_perror("/tmp/heredoc");
//         return (1);
//     }
//     fd_read = open("/tmp/heredoc", O_RDONLY);
//     if (fd_read < 0)
//     {
//         tt();
//         tt();
//         close(fd_write); // Close the write fd if read fails
//         ft_perror("/tmp/heredoc");
//         return (1);
//     }
//     unlink("/tmp/heredoc"); // Remove the file after opening it
//     signal(SIGINT, handle_heredoc_signal); // Set signal handler for SIGINT
//     while (1)
//     {
//         char *line = readline("> ");
//         if (g_status == 130) // Check if SIGINT was received
//         {
//             close(fd_write);
//             close(fd_read);
//             return (1); // Exit on SIGINT
//         }
//         if (!line || strcmp(line, cmd->redirs->heredoc->delimiter) == 0)
//         {
//             free(line);
//             close(fd_write);
//             close(fd_read);
//             return (0); // Exit loop on delimiter match
//         }
//         if (cmd->redirs->heredoc->flag == 0)
//         {
//             char *expanded = expand_heredoc_content(line, &env_list, g_status, cmd->redirs->heredoc->delimiter);
//             if (expanded)
//             {
//                 ft_putstr_fd(expanded, fd_write);
//                 free(expanded);
//             }
//         }
//         else
//         {
//             ft_putstr_fd(line, fd_write);
//         }
//         ft_putstr_fd("\n", fd_write);
//         free(line);
//     }
//     close(fd_write);
     
// }