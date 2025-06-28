/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:21:12 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 18:43:32 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool is_redirection(char *cmd)
{
    return (strchr(cmd, LLESS_THAN_SIGNESS) || strchr(cmd, GREAGREATER_THAN_SIGN) ||
            strstr(cmd, DOUBLE_GREATER_THAN_SIGN) || strstr(cmd, DOUBLE_LESS_THAN_SIGN));
}


