/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:56:00 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 21:18:03 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	env_to_list(t_env **env_list, char **env)
{
	t_env	*new_node;
	int		i;

	i = 0;
	while (env[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;
		new_node->key = ft_strdup(env[i]);
		new_node->value = ft_strchr(new_node->key, EQEQUALS_SIGNUAL);
		if (new_node->value)
		{
			*new_node->value = '\0';
			new_node->value++;
		}
		else
			new_node->value = NULL;
		new_node->export_variable = true;
		new_node->next = *env_list;
		*env_list = new_node;
		i++;
	}
}

t_env	*search_env_node(t_env *list_head, const char *search_key)
{
	t_env	*traverser;

	traverser = list_head;
	while (traverser)
	{
		if (!ft_strcmp(traverser->key, search_key))
			return (traverser);
		traverser = traverser->next;
	}
	return (NULL);
}

void	configure_environment(t_env **env_list, char **env_array)
{
	t_env	*found_node;

	env_to_list(env_list, env_array);
	found_node = search_env_node(*env_list, "_");
	if (!found_node)
		add_env_value(env_list, "_", "/usr/bin/env");
	found_node = search_env_node(*env_list, "SHLVL");
	if (!found_node)
		add_env_value(env_list, "SHLVL", "1");
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
