/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:06 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 17:57:15 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	sort_env_list(t_env **env_list)
{
	t_env	*current;
	t_env	*next;
	char	*temp_key;
	char	*temp_value;

	current = *env_list;
	if (!current)
		return ;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (ft_strcmp(current->key, next->key) > 0)
			{
				temp_key = current->key;
				current->key = next->key;
				next->key = temp_key;

				temp_value = current->value;
				current->value = next->value;
				next->value = temp_value;
			}
			next = next->next;
		}
		current = current->next;
	}

}

void	export_one_args(t_env **env_list)
{
	t_env	*current;

	sort_env_list(env_list);
	current = *env_list;
	while (current)
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
		current = current->next;
	}
}

bool	is_append(const char *arg)
{
	return (ft_strstr(arg, "+=") != NULL);
}

void	append_env_value(t_env **env_list, const char *key, const char *value)
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
			free(current->value);
			current->value = new_value;
			return ;
		}
		current = current->next;
	}
	add_env_value(env_list, new_key, value);
}

void	add_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->is_exported = true;
	new_node->next = *env_list;
	*env_list = new_node;
}
void	add_temporary_env_value(t_env **env_list, const char *key)
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
	new_node->is_exported = false;
	new_node->next = *env_list;
	*env_list = new_node;
}

void	export(t_cmd *cmd, t_env **env_list)
{
	int i ;
	char *key;
	char *value;
	
	if (!cmd->args || !cmd->args[1])
	{
		export_one_args(env_list);
		return ;
	}
	i = 1;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '=') != NULL)
		{
				key = ft_substr(cmd->args[i], 0, ft_strchr(cmd->args[i], '=') - cmd->args[i]);
			value = ft_strchr(cmd->args[i], '=') + 1;
			if (is_append(cmd->args[i]))
				append_env_value(env_list, key, value);
			else
				add_env_value(env_list, key, value);
			free(key);
		}
		else
			add_temporary_env_value(env_list, cmd->args[i]);
		i++;
	}
}

