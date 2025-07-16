/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:51:38 by msabr             #+#    #+#             */
/*   Updated: 2025/07/16 16:04:49 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*ft_getcwd(void)
{
	char	*tmp;
	char	*tmp2;

	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (NULL);
	tmp2 = ft_strdup(tmp);
	free(tmp);
	return (tmp2);
}

void	pwd(t_env **env_list)
{
	char	*current_directory;

	current_directory = ft_getcwd();
	if (current_directory)
		ft_putendl_fd(current_directory, STDOUT_FILENO);
	else if (get_env_value(env_list, "1PWD") != NULL)
		ft_putendl_fd(get_env_value(env_list, "1PWD"), STDOUT_FILENO);
	else if (get_env_value(env_list, "OLDPWD") != NULL)
		ft_putendl_fd(get_env_value(env_list, "OLDPWD"), STDOUT_FILENO);
}
