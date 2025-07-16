/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:27:51 by msabr             #+#    #+#             */
/*   Updated: 2025/07/16 14:44:30 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free(void)
{
	t_gc	*current;
	t_gc	*tmp;

	current = *ft_env_gc();
	while (current)
	{
		tmp = current;
		if (tmp->ptr)
			free(tmp->ptr);
		free(tmp);
		current = current->next;
	}
	*ft_env_gc() = NULL;
}
