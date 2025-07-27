/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_overwrite.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:11 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 13:23:51 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirection.h"

int	redirect_overwrite(char *file_name)
{
	int	fd;

	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_perror(file_name);
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
