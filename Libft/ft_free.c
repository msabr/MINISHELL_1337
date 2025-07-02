/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:27:51 by msabr             #+#    #+#             */
/*   Updated: 2025/07/02 15:54:07 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_gc	**ft_env_gc(void)
{
	static t_gc *gc = NULL;

	return (&gc);
}

void	ft_free(void)
{
	t_gc	*current;
	t_gc	*tmp;

	current = *ft_env_gc();
	while (current)
	{
		tmp = current;
		current = current->next;
		free(tmp->ptr);
		free(tmp);
	}
	*ft_env_gc() = NULL;
}
