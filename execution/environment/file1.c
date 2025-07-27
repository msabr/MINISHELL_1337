/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:56:00 by msabr             #+#    #+#             */
/*   Updated: 2025/07/27 13:05:49 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

static t_env	*create_env_node(char *env_entry)
{
	t_env	*new_node;

	new_node = ft_malloc(sizeof(t_env));
	new_node->key = ft_strdup(set_key(env_entry));
	if (!new_node->key)
		return (NULL);
	new_node->value = ft_strdup(env_entry + ft_strlen(new_node->key) + 1);
	if (!new_node->value)
		return (NULL);
	if (ft_strchr(env_entry, '='))
		new_node->export_variable = true;
	else
		new_node->export_variable = false;
	new_node->next = NULL;
	return (new_node);
}

t_env	*env_to_list(char **env)
{
	t_env	*env_list;
	t_env	*new_node;
	t_env	*temp;
	int		i;

	env_list = NULL;
	i = -1;
	while (env[++i])
	{
		new_node = create_env_node(env[i]);
		if (!new_node)
			return (NULL);
		if (!env_list)
			env_list = new_node;
		else
		{
			temp = env_list;
			while (temp->next)
				temp = temp->next;
			temp->next = new_node;
		}
	}
	return (env_list);
}

static char	*fill_env_array(char *env, t_env *env_list)
{
	env = ft_strjoin(env_list->key, "=");
	if (env_list->value)
		env = ft_strjoin(env, env_list->value);
	return (env);
}

char	**list_to_env(t_env *env_list)
{
	t_env	*traverser;
	int		count;
	char	**env_array;

	count = size_of_env_list(env_list);
	env_array = (char **)ft_malloc(sizeof(char *) * (count + 1));
	traverser = env_list;
	count = 0;
	while (traverser)
	{
		if (traverser->export_variable)
		{
			env_array[count] = fill_env_array(env_array[count], traverser);
			if (!env_array[count])
				return (NULL);
			count++;
		}
		traverser = traverser->next;
	}
	env_array[count] = NULL;
	return (env_array);
}

t_env	*find_env_node(char *search_key, t_env *list_head)
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
