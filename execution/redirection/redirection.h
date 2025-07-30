/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 13:10:33 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 13:11:41 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include "../execution.h"

# include <sys/ioctl.h>
# include <fcntl.h>

# define MAX_HEREDOCS 16

int		redirect_stdin(char *file_name);
int		redirect_overwrite(char *file_name);
int		redirect_append(char *file_name);
int		redirect_heredoc(t_redir *redirs, t_env *env);
int		heredoc_loop(t_heredoc *heredoc);
void	heredoc_content_handler(t_heredoc *heredoc, char *line);
int		open_file_heredoc(t_heredoc *heredoc);
char	*ft_readline_heredoc(const char *prompt);
void	handle_heredoc_signal(int sig);
char	*get_temp_filename(void);
void	check_nbr_heredocs(t_cmd *cmds);

#endif