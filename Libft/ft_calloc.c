/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 02:02:15 by msabr             #+#    #+#             */
/*   Updated: 2024/11/18 19:26:14 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*p;
	size_t			len;

	len = count * size;
	if (size != 0 && (count != len / size))
		return (NULL);
	p = malloc(len);
	if (!p)
		return (NULL);
	ft_bzero(p, len);
	return (p);
}
