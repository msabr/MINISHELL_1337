/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 13:10:33 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 13:50:47 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# include <sys/ioctl.h>

# include "../execution.h"

bool	valid_filename(const char *filename);
int		redirect_stdin(char *file_name);
int		redirect_overwrite(char *file_name);
int		redirect_append(char *file_name);
int		redirect_heredoc(t_redir *redirs, t_env *env);
void	check_nbr_heredocs(t_cmd *cmds);
char	*get_temp_filename(void);
void	handle_heredoc_signal(int sig);

#endif