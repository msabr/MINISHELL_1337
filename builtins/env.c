/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:50 by msabr             #+#    #+#             */
/*   Updated: 2025/06/15 19:15:02 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//env with no options or arguments
void env_function(t_env *env_list)
{
    t_env *current = env_list;
    char *result;

    while (current)
    {
        if (current->value)
        {
            result = ft_strjoin(current->key, "=");
            result = ft_strjoin(result, current->value);
            ft_putendl_fd(result, STDOUT_FILENO);
            free(result);
        
        }
        else
        {
            ft_putendl_fd(current->key, STDOUT_FILENO);
        }
        current = current->next;
    }
}
