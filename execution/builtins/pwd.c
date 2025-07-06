/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:51:38 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 18:58:56 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	pwd(t_env **env_list)
{
	char	*current_directory;

	current_directory = getcwd(NULL, 0);
	if (current_directory)
	{
		ft_putendl_fd(current_directory, STDOUT_FILENO);
	}
	else if (get_env_value(env_list, "1PWD") != NULL)
	{
		ft_putendl_fd(get_env_value(env_list, "1PWD"), STDOUT_FILENO);
	}
	else if (get_env_value(env_list, "OLDPWD") != NULL)
	{
		ft_putendl_fd(get_env_value(env_list, "OLDPWD"), STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: pwd: error retrieving current directory: \
No such file or directory\n", STDERR_FILENO);
	}
}
