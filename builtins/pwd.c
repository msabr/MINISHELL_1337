/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:51:38 by msabr             #+#    #+#             */
/*   Updated: 2025/06/15 19:55:41 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//pwd with no options
void pwd(void)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd)
        ft_putendl_fd(cwd, STDOUT_FILENO);
    else
        perror("pwd");
    free(cwd);
}
