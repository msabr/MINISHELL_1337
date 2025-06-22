/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:26 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 18:04:54 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//unset with no options
void unset(t_cmd *cmd, t_env **env_list)
{
    int i;

    i = 1;
    while (cmd->args && cmd->args[i])
    {
        if (check_env_variable(env_list, cmd->args[i]) == 0)
        {
            remove_env_variable(env_list, cmd->args[i]);
        }
        cmd->args++;
    }
}
