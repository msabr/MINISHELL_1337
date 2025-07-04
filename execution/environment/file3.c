/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:16:36 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 15:17:39 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_shlvl(t_env *node, t_env ***env_list)
{
	int		shlvl_value;
	char	*new_value;

	if (!node || !node->value)
		return ;
	shlvl_value = ft_atoi(node->value);
	if (shlvl_value < 0)
	{
		ft_putstr_fd("minishell: warning: SHLVL is negative, resetting to 0\n",
			STDERR_FILENO);
		new_value = ft_itoa(0);
	}
	else
	{
		new_value = ft_itoa(shlvl_value + 1);
	}
	if (!new_value)
		return ;
	add_env_value(*env_list, "SHLVL", new_value);
}

void	configure_environment(t_env **env_list, char **env_array)
{
	t_env	*found_node;

	*env_list = env_to_list(env_array);
	found_node = search_env_node(*env_list, "SHLVL");
	if (!found_node)
		add_env_value(env_list, "SHLVL", "1");
	else
		handle_shlvl(found_node, &env_list);
	found_node = search_env_node(*env_list, "PWD");
	if (!found_node)
		add_env_value(env_list, "PWD", get_pwd());
	found_node = search_env_node(*env_list, "PATH");
	if (!found_node)
		add_env_value(env_list, "PATH",
			"/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	found_node = search_env_node(*env_list, "OLDPWD");
	if (!found_node)
		add_temporary_env_value(env_list, "OLDPWD");
}
