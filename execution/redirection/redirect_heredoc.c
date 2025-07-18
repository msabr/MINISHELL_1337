/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 20:17:55 by msabr             #+#    #+#             */
/*   Updated: 2025/07/18 23:37:00 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// void	handel_herdoc(int sig)
// {
// 	if (sig == SIGINT)
// 		close(0);
// }

// int	write_to_file(char *buffer)
// {
// 	char	*file_tmp;
// 	int		i;
// 	int		fd;
// 	int		fd_read;

// 	file_tmp = ft_strdup("herdoc.txt");
// 	i = 0;
// 	while (access(file_tmp, F_OK) != -1)
// 		file_tmp = ft_strjoin("herdoc.txt", ft_itoa(i++));
// 	fd = open(file_tmp, O_CREAT | O_RDWR, 0777);
// 	fd_read = open(file_tmp, O_RDWR, 0777);
// 	if (fd < 0)
// 		write(2, "Error\n", 6);
// 	if (fd_read < 0)
// 		write(2, "Error\n", 6);
// 	if (!buffer)
// 		buffer = ft_strdup("");
// 	write(fd, buffer, ft_strlen(buffer));
// 	close(fd);
// 	unlink(file_tmp);
// 	return (fd_read);
// }
// char *add_cmd_dn_buffer(char *cmd, char *buffer)
// {
// 	if (!cmd)
// 		cmd = ft_strdup("");
// 	if (!buffer)
// 		buffer = ft_strdup("");
// 	buffer = strjoin_and_free(buffer, cmd);
// 	buffer = strjoin_and_free(buffer, "\n");
// 	return (buffer);
// }

// char *expand_vars_and_heredoc(char *cmd, t_env *env)
// {
// 	char	*expanded_cmd;
// 	char	*heredoc_content;

// 	expanded_cmd = expand_dollars(cmd, &env);
// 	heredoc_content = expand_heredoc_content(expanded_cmd, &env, 0, NULL);
// 	// free(expanded_cmd);
// 	return (heredoc_content);
// }

// int	readline_hdc(char *delimiter, t_env *env, int flag)
// {
// 	char	*cmd;
// 	char	*cmd_copy;
// 	char	*buffer;

// 	buffer = NULL;
// 	signal(SIGINT, handel_herdoc);
// 	while (1)
// 	{
// 		cmd = readline("> ");
// 		cmd_copy = cmd;
// 		if (!ttyname(0))
// 			return (open(ttyname(2), O_RDWR), -3);
// 		if (!cmd)
// 			return (write_to_file(buffer));
// 		if (!ft_strcmp(cmd, delimiter))
// 		{
// 			// free(cmd_copy);
// 			break ;
// 		}
// 		if (flag != 42)
// 			cmd = expand_vars_and_heredoc(cmd, env);
// 		buffer = add_cmd_dn_buffer(cmd, buffer);
// 		// free(cmd_copy);
// 	}
// 	return (write_to_file(buffer));
// }

// int verify_cmd(t_cmd *cmd, char **args, bool is_builtin)
// {
// 	if (!cmd || !cmd->redirs || !cmd->redirs->filename)
// 	{
// 		if (is_builtin)
// 			return (0);
// 		else
// 			return (print_execve_error(args ? args[0] : "command"));
// 	}
// 	if (cmd->redirs->type == TOKEN_HEREDOC && !cmd->redirs->filename)
// 	{
// 		// cmd->redirs->exit_status = 1;
// 		return (-1);
// 	}
// 	return (0);
// }

// char	*ft_delimiter(t_cmd *cmd, int *flag)
// {
// 	char	*delimiter;

// 	if (!cmd || !cmd->redirs || !cmd->redirs->filename)
// 	{
// 		*flag = 1;
// 		return (NULL);
// 	}
// 	delimiter = cmd->redirs->filename;
// 	if (!delimiter || !*delimiter)
// 	{
// 		*flag = 1;
// 		return (NULL);
// 	}
// 	if (ft_strchr(delimiter, ' ') || ft_strchr(delimiter, '\t'))
// 	{
// 		write(2, "minishell: syntax error near unexpected token `newline'\n", 58);
// 		*flag = 1;
// 		return (NULL);
// 	}
// 	return (delimiter);
// }

// int	redirect_heredoc(t_cmd  *cmd, t_env *env)
// {
// 	int	flag;
// 	char	*delimiter;
	
// 	while (cmd && cmd->redirs)
// 	{
// 		flag = 0;
// 		if (cmd->redirs->type == TOKEN_HEREDOC)
// 		{
// 			if (verify_cmd(cmd, NULL, false) == -1)
// 				break ;
// 			delimiter = ft_delimiter(cmd, &flag);
// 			if (!delimiter)
// 				break ;
// 			cmd->redirs->fd_in = readline_hdc(delimiter, env, flag);
// 			if (cmd->redirs->fd_in == -3)
// 			{
// 				// cmd->redirs->exit_status = 1;
// 				return (-1);
// 			}
// 		}
// 		if (flag)
// 			return (-1);
// 		cmd = cmd->next;
// 	}
// 	return (0);
// }

// int redirect_heredoc(t_cmd *cmd, t_env *env)
// {
//     t_redir *current = cmd->redirs;
//     t_heredoc *heredoc;
//     char *line;
//     int pipe_fd[2];
//     char *expanded_line;

//     while (current)
//     {
//         if (current->type == TOKEN_HEREDOC)
//         {
//             heredoc = current->heredoc;
//             if (pipe(pipe_fd) == -1)
//             {
//                 perror("minishell: pipe");
//                 return (1);
//             }
//             while (1)
//             {
//                 line = readline("> ");
//                 if (!line || ft_strcmp(line, heredoc->ldelimiter) == 0)
//                 {
//                     free(line);
//                     break;
//                 }
//                 if (heredoc->flag == 0) // Not quoted, expand variables
//                 {
//                     expanded_line = expand_heredoc_content(line, &env, *ft_get_status(), heredoc->delimiter);
//                     if (expanded_line)
//                     {
//                         ft_putstr_fd(expanded_line, pipe_fd[1]);
//                         free(expanded_line);
//                     }
//                 }
//                 else // Quoted, no expansion
//                 {
//                     ft_putstr_fd(line, pipe_fd[1]);
//                 }
//                 ft_putstr_fd("\n", pipe_fd[1]);
//                 free(line);
//             }
//             close(pipe_fd[1]);
//             if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
//             {
//                 perror("minishell: dup2");
//                 close(pipe_fd[0]);
//                 return (1);
//             }
//             close(pipe_fd[0]);
//             heredoc->fd_read = STDIN_FILENO;
//         }
//         current = current->next;
//     }
//     return (0);
// }

#include "../../minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int redirect_heredoc(t_cmd *cmd, t_env *env)
{
    t_redir *current = cmd->redirs;
    t_heredoc *heredoc;
    char *line;
    int fd;
    char *tmp_file = "/tmp/minishell_heredoc_tmp";
    
    while (current)
    {
        tt();
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