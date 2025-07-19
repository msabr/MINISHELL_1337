/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:43:13 by msabr             #+#    #+#             */
/*   Updated: 2025/07/19 15:45:09 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	*ft_get_status(void)
{
	static int	status;

	return (&status);
}

void	ft_set_status(int status)
{
	int	*ptr;

	ptr = ft_get_status();
	*ptr = status;
}

void	ft_exit(int status)
{
    ft_free();
    rl_clear_history();
    exit(status);
}

void	set_and_exit(int status)
{
	ft_set_status(status);
	ft_exit(status);
}