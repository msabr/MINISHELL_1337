/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:56:00 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 15:17:52 by msabr            ###   ########.fr       */
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

static char	*fill_env_array(char *env, t_env *env_list)
{
	env = ft_strdup(env_list->key);
	if (env_list->export_variable)
		env = ft_strjoin(env, "=");
	if (!env)
		return (NULL);
	if (env_list->value)
		env = ft_strjoin(env, env_list->value);
	return (env);
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
		env_array[count] = fill_env_array(env_array[count], traverser);
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
