/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:26 by msabr             #+#    #+#             */
/*   Updated: 2025/06/15 15:57:03 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"




//unset with no options
void unset(t_cmd *cmd, t_env *env_list)
{
    if (!cmd->args || !cmd->args[0])
    {
        ft_putstr_fd("unset: not enough arguments\n", STDERR_FILENO);
        return;
    }

    for (int i = 0; cmd->args[i]; i++)
    {
        if (check_env_variable(env_list, cmd->args[i]) == 0)
        {
            remove_env_variable(env_list, cmd->args[i]);
        }
    }
}
