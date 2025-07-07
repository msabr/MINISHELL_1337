/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:06 by msabr             #+#    #+#             */
/*   Updated: 2025/07/05 23:29:16 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static void	append_env_value(t_env **env_list, char *key, char *value)
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
			return ;
		}
		current = current->next;
	}
	add_env_value(env_list, new_key, value);
}

static void	print_error_export(char *arg)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

void	export(t_cmd *cmd, t_env **env_list)
{
	int		i ;
	char	*key;
	char	*value;

	i = 1;
	if (!cmd->args || !cmd->args[i])
		return (export_withot_args(*env_list));
	while (cmd->args[i])
	{
		if (!is_valid_key_export(cmd->args[i]))
			print_error_export(cmd->args[i]);
		else if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			key = set_key(cmd->args[i]);
			value = ft_strchr(cmd->args[i], '=') + 1;
			if (ft_strstr(cmd->args[i], "+="))
				append_env_value(env_list, key, value);
			else
				add_env_value(env_list, key, value);
		}
		else
			add_temporary_env_value(env_list, cmd->args[i]);
		i++;
	}
}
