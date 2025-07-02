/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:51:38 by msabr             #+#    #+#             */
/*   Updated: 2025/07/01 18:07:23 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_pwd(void)
{
	char	*current_directory;

	current_directory = getcwd(NULL, 0);
	if (!current_directory)
	{
		perror("getcwd");
		return (NULL);
	}
	return (current_directory);
}

char	*get_pwd_from_env(t_env **env_list)
{
	t_env	*pwd_node;

	pwd_node = search_env_node(*env_list, "PWD");
	if (pwd_node && pwd_node->value)
	{
		return (ft_strdup(pwd_node->value));
	}
	return (NULL);
}

void	pwd(t_env **env_list)
{
	char	*current_directory;

	current_directory = get_pwd();
	if (current_directory)
	{
		ft_putendl_fd(current_directory, STDOUT_FILENO);
	}
	else
	{
		ft_putendl_fd(get_pwd_from_env(env_list), STDOUT_FILENO);
	}
}
