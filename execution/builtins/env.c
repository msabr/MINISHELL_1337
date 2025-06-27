/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:50 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 18:45:10 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	env_function(t_env *env_list)
{
	t_env	*current;
	char	*result;

	current = env_list;
	while (current)
	{
		if (current->value && current->declared_only)
		{
			result = ft_strjoin(current->key, "=");
			result = ft_strjoin(result, current->value);
			ft_putendl_fd(result, STDOUT_FILENO);
			free(result);
		}
		else if (current->declared_only)
		{
			result = ft_strjoin(current->key, "=");
			ft_putendl_fd(result, STDOUT_FILENO);
		}
		current = current->next;
	}
}
