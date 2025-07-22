/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 17:24:49 by msabr             #+#    #+#             */
/*   Updated: 2025/07/22 18:15:31 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_exit(int status);

t_gc	**ft_env_gc(void)
{
	static t_gc	*gc;

	return (&gc);
}

void	*ft_malloc(size_t size)
{
	t_gc	*new;
	void	*ptr;
	t_gc	**gc;

	if (size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
		ft_exit(1);
	new = malloc(sizeof(t_gc));
	if (!new)
		ft_exit(1);
	gc = ft_env_gc();
	new->ptr = ptr;
	if (*gc)
		new->next = *gc;
	else
		new->next = NULL;
	*gc = new;
	return (ptr);
}
