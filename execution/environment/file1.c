/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:56:00 by msabr             #+#    #+#             */
/*   Updated: 2025/07/02 14:41:05 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_env	*env_to_list(char **env)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	i = -1;
	while (env[++i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_strdup(set_key(env[i]));
		if (!new_node->key)
			return (NULL);
		new_node->value = ft_strdup(env[i] + ft_strlen(new_node->key) + 1);
		if (!new_node->value)
			return (NULL);
		if (ft_strchr(env[i], '='))
			new_node->export_variable = true;
		else
			new_node->export_variable = false;
		new_node->next = env_list;
		env_list = new_node;
	}
	return (env_list);
}

char	**list_to_env(t_env *env_list)
{
	t_env	*traverser;
	int		count;
	char	**env_array;

	count = 0;
	traverser = env_list;
	while (traverser)
	{
		count++;
		traverser = traverser->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	count = 0;
	traverser = env_list;
	while (traverser)
	{
		env_array[count] = ft_strdup(traverser->key);
		if (traverser->export_variable)
			env_array[count] = ft_strjoin(env_array[count], "=");
		if (!env_array[count])
			return (NULL);
		if (traverser->value)
			env_array[count] = ft_strjoin(env_array[count], traverser->value);
		count++;
		traverser = traverser->next;
	}
	env_array[count] = NULL;
	return (env_array);
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
void	handle_shlvl(t_env *node, t_env ***env_list)
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
	found_node = search_env_node(*env_list, "_");
	if (!found_node)
		add_env_value(env_list, "_", "/usr/bin/env");
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
