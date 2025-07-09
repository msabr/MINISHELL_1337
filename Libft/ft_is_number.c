/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_number.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 19:36:45 by msabr             #+#    #+#             */
/*   Updated: 2025/07/09 15:04:37 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	ft_is_number(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[i] == '\0')
		return (false);
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			break ;
		i++;
	}
	while (ft_isspace(str[i]))
		i++;
	if (str[i] != '\0')
		return (false);
	return (true);
}
