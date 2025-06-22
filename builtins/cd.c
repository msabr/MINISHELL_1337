/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:51:05 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 20:27:57 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static void	set_env_value(t_env *env_list, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_entry;

	current = env_list;
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
	new_entry = malloc(sizeof(t_env));
	new_entry->key = ft_strdup(key);
	new_entry->value = ft_strdup(value);
	new_entry->next = env_list;
	env_list = new_entry;
}

void	cd(t_cmd *cmd, t_env *env_list)
{
	char	*path;
	char	*cwd;

	if (!cmd->args || !cmd->args[1])
	{
		path = get_env_value(env_list, "HOME");
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return ;
		}
	}
	else
	{
		path = cmd->args[1];
	}
	if (chdir(path) == -1)
		return (perror("cd"));
	set_env_value(env_list, "OLDPWD", get_env_value(env_list, "PWD"));
	cwd = getcwd(NULL, 0);
	if (!cwd)
		set_env_value(env_list, "PWD", cwd);
}
