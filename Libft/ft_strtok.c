/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:32:16 by msabr             #+#    #+#             */
/*   Updated: 2025/06/23 13:35:07 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strtok(char *str, const char *delim)
{
	static char	*next;
	char		*start;
	int			i;

	i = 0;
	if (str)
		next = str;
	if (!next || *next == '\0')
		return (NULL);
	while (*next && strchr(delim, *next))
		next++;
	if (*next == '\0')
		return (NULL);
	start = next;
	while (*next && !strchr(delim, *next))
		next++ ;
	if (*next)
	{
		*next = '\0';
		next++ ;
	}
	else
		next = NULL;
	return (start);
}
