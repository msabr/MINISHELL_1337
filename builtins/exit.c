/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:53:11 by msabr             #+#    #+#             */
/*   Updated: 2025/06/14 15:29:03 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//exit with no options
void exit_shell(t_cmd *cmd, t_env *env_list)
{
    int exit_code = 0;

    if (cmd->args && cmd->args[0])
    {
        exit_code = ft_atoi(cmd->args[0]);
        if (exit_code == 0 && ft_strcmp(cmd->args[0], "0") != 0)
        {
            ft_putstr_fd("exit: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
            exit_code = 255; // Default exit code for invalid input
        }
    }

    free_env_list(env_list); // Free the environment list
    exit(exit_code);
}
