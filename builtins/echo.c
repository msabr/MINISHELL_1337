/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:41:10 by msabr             #+#    #+#             */
/*   Updated: 2025/06/14 11:41:51 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void echo(t_cmd *cmd)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    if (cmd->args && cmd->args[0] && ft_strcmp(cmd->args[0], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (cmd->args && cmd->args[i])
    {
        ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
        if (cmd->args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
}