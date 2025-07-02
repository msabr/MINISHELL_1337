/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 22:47:00 by msabr             #+#    #+#             */
/*   Updated: 2025/07/02 18:10:01 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	a->key = b->key;
	b->key = temp_key;
	temp_value = a->value;
	a->value = b->value;
	b->value = temp_value;
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

	new_key = ft_malloc(ft_strlen(key));
	ft_strlcpy(new_key, key, ft_strlen(key));
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, new_key) == 0)
		{
			new_value = ft_strjoin(current->value, value);
			current->value = new_value;
			return ;
		}
		current = current->next;
	}
	add_env_value(env_list, new_key, value);
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
	if (!is_valid_key(key))
		return (NULL);
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
