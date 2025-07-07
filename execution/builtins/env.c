/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:50 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 19:23:22 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	env_function(t_env *env_list)
{
	t_env	*current;
	char	*result;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "1PWD"))
		{
			if (current->value && current->export_variable)
			{
				result = ft_strjoin(current->key, "=");
				result = ft_strjoin(result, current->value);
				ft_putendl_fd(result, STDOUT_FILENO);
			}
			else if (current->export_variable)
			{
				result = ft_strjoin(current->key, "=");
				ft_putendl_fd(result, STDOUT_FILENO);
			}
		}
		current = current->next;
	}
}
