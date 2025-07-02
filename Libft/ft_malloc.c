/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:24:49 by msabr             #+#    #+#             */
/*   Updated: 2025/07/02 15:52:56 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_gc	**ft_env_gc(void)
{
	static t_gc *gc = NULL;

	return (&gc);
}

void	*ft_malloc(size_t size)
{
	t_gc	*new;
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	new = malloc(sizeof(t_gc));
	if (!new)
		return (NULL);
	new->ptr = ptr;
	new->next = *ft_env_gc();
	*ft_env_gc() = new;
	return (ptr);
}

void	*ft_malloc_env(size_t size)
{
	if (size == 0)
		return (NULL);
	return (ft_malloc(size));
}
