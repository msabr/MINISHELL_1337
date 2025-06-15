/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:50 by msabr             #+#    #+#             */
/*   Updated: 2025/06/14 15:22:07 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//env with no options or arguments
void env_function(t_env *env_list)
{
    t_env *current = env_list;

    while (current)
    {
        if (current->value)
        {
            ft_putstr_fd(current->key, STDOUT_FILENO);
            ft_putchar_fd('=', STDOUT_FILENO);
            ft_putstr_fd(current->value, STDOUT_FILENO);
        }
        else
        {
            ft_putstr_fd(current->key, STDOUT_FILENO);
        }
        ft_putchar_fd('\n', STDOUT_FILENO);
        current = current->next;
    }
}
