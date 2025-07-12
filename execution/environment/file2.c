/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:03:04 by msabr             #+#    #+#             */
/*   Updated: 2025/07/12 18:05:43 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_env_value(t_env **env_list, const char *key)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	add_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			current->value = ft_strdup(value);
			current->export_variable = true;
			return ;
		}
		current = current->next;
	}
	new_node = ft_malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->export_variable = true;
	new_node->next = *env_list;
	*env_list = new_node;
}

void	add_temporary_env_value(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*new_node;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return ;
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = NULL;
	new_node->export_variable = false;
	new_node->next = *env_list;
	*env_list = new_node;
}

int	remove_env_variable(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env_list = current->next;
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (-1);
}
