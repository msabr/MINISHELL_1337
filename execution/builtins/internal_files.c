/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:17:03 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 19:22:36 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	is_valid_key_export(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (false);
	while (key[i] && key[i] != '=')
	{
		if (key[i] == '+' && key[i + 1] != '=')
			return (false);
		if (!ft_isalnum(key[i]) && key[i] != '_' && key[i] != '+')
			return (false);
		i++;
	}
	return (true);
}

bool	is_valid_key_unset(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (false);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}
