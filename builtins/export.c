/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:06 by msabr             #+#    #+#             */
/*   Updated: 2025/06/14 15:07:19 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//export with no options
void export(t_cmd *cmd, t_env *env_list)
{
    if (!cmd->args || !cmd->args[0])
    {
        // Print all environment variables
        t_env *current = env_list;
        while (current)
        {
            ft_putstr_fd("declare -x ", STDOUT_FILENO);
            ft_putstr_fd(current->key, STDOUT_FILENO);
            if (current->value)
            {
                ft_putchar_fd('=', STDOUT_FILENO);
                ft_putstr_fd("\"", STDOUT_FILENO);
                ft_putstr_fd(current->value, STDOUT_FILENO);
                ft_putstr_fd("\"", STDOUT_FILENO);
            }
            ft_putchar_fd('\n', STDOUT_FILENO);
            current = current->next;
        }
    }
    else
    {
        // Add or update environment variables
        for (int i = 0; cmd->args[i]; i++)
        {
            char *equal_sign = ft_strchr(cmd->args[i], '=');
            if (equal_sign)
            {
                *equal_sign = '\0';
                set_env_value(env_list, cmd->args[i], equal_sign + 1);
            }
            else
            {
                set_env_value(env_list, cmd->args[i], "");
            }
        }
    }
}
