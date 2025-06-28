/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:17:03 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 18:45:10 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != UNDERSCORE)
		return (false);
	while (key[i] && key[i] != '=')
	{
		if (key[i] == '+' && key[i + 1] != '=')
			return (false);
		if (!ft_isalnum(key[i]) && key[i] != UNDERSCORE && key[i] != '+')
			return (false);
		i++;
	}
	return (true);
}
