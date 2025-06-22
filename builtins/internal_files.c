/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal _files.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 20:17:03 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 20:30:52 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	is_valid_key(char *key)
{
	if (ft_isalpha(key[0]) || key[0] == UNDERSCORE)
		return (true);
	return (false);
}
