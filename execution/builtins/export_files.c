/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 22:47:00 by msabr             #+#    #+#             */
/*   Updated: 2025/07/05 18:20:15 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	swap_env_nodes(t_env *a, t_env *b)
{
	t_env	*temp;

	temp = malloc(sizeof(t_env));
	temp->key = a->key;
	temp->value = a->value;
	temp->export_variable = a->export_variable;
	a->key = b->key;
	a->value = b->value;
	a->export_variable = b->export_variable;
	b->key = temp->key;
	b->value = temp->value;
	b->export_variable = temp->export_variable;
	// free(temp);
}

void	sort_env_list(t_env **env_list)
{
	t_env	*current;
	t_env	*next;

	current = *env_list;
	if (!current)
		return ;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcmp(current->key, next->key) > 0)
				swap_env_nodes(current, next);
			next = next->next;
		}
		current = current->next;
	}
}

void	append_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;
	char	*new_value;
	char	*new_key;

	new_key = malloc(ft_strlen(key));
	ft_strlcpy(new_key, key, ft_strlen(key));
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, new_key) == 0)
		{
			new_value = ft_strjoin(current->value, value);
			current->value = new_value;
			// free(|);
			return ;
		}
		current = current->next;
	}
	add_env_value(env_list, new_key, value);
	// free(new_key);
}

char	*set_key(const char *arg)
{
	char	*key;
	size_t	i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	key = ft_malloc(i + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg, i + 1);
	return (key);
}

void	export_withot_args(t_env **env_list)
{
	t_env	*current;

	sort_env_list(env_list);
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, "_"))
		{
			if (current->value)
			{
				ft_putstr_fd("declare -x ", STDOUT_FILENO);
				ft_putstr_fd(current->key, STDOUT_FILENO);
				ft_putstr_fd("=\"", STDOUT_FILENO);
				ft_putstr_fd(current->value, STDOUT_FILENO);
				ft_putstr_fd("\"\n", STDOUT_FILENO);
			}
			else
			{
				ft_putstr_fd("declare -x ", STDOUT_FILENO);
				ft_putstr_fd(current->key, STDOUT_FILENO);
				ft_putstr_fd("\n", STDOUT_FILENO);
			}
		}
		current = current->next;
	}
}
