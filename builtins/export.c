/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:06 by msabr             #+#    #+#             */
/*   Updated: 2025/06/21 19:44:39 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//export with no options
void sort_env_list(t_env **env_list)
{
	t_env *current;
	t_env *next;
	char *temp_key;
	char *temp_value;

	if (!env_list || !*env_list)
		return;

	for (current = *env_list; current; current = current->next)
	{
		for (next = current->next; next; next = next->next)
		{
			if (ft_strcmp(current->key, next->key) > 0)
			{
				// Swap keys and values
				temp_key = current->key;
				temp_value = current->value;
				current->key = next->key;
				current->value = next->value;
				next->key = temp_key;
				next->value = temp_value;
			}
		}
	}
}

void export_one_args(t_env **env_list)
{
	sort_env_list(env_list);
	t_env *current = *env_list;
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
bool is_append(const char *arg)
{
	// "+=" in args if 
	return (ft_strstr(arg, "+=") != NULL);
}
void append_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env *current = *env_list;

	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			char *new_value = ft_strjoin(current->value, value);
			free(current->value);
			current->value = new_value;
			return;
		}
		current = current->next;
	}
	add_env_value(env_list, key, value); // If not found, add as a new variable
}



void add_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env *current = *env_list;

	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}

	t_env *new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->is_exported = true;
	new_node->next = *env_list;
	*env_list = new_node;
}



void export(t_cmd *cmd, t_env **env_list)
{
	int i ;
	char *key;
	char *value;


	if (!cmd->args[1])
		export_one_args(env_list);
	else
	{
		i = 1;
		while (cmd->args[i])
		{
			if (ft_strchr(cmd->args[i], '=') == NULL)
			{
				ft_putstr_fd("export: `", STDERR_FILENO);
				ft_putstr_fd(cmd->args[i], STDERR_FILENO);
				ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
				i++;
				continue;
			}

			if (is_append(cmd->args[i]))
			{
				key = ft_substr(cmd->args[i], 0, ft_strchr(cmd->args[i], '=') - cmd->args[i] - 1);
				value = ft_strchr(cmd->args[i], '=') + 1;
				append_env_value(env_list, key, value);
				printf("exported %s with value %s\n", key, value);
			}
			else
			{
				key = ft_substr(cmd->args[i], 0, ft_strchr(cmd->args[i], '=') - cmd->args[i]);
				value = ft_strchr(cmd->args[i], '=') + 1;
				add_env_value(env_list, key, value);

			}

			free(key);
			i++;
		}
	}

}

