/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 04:59:26 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 12:54:42 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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

void	print_error_export(char *arg, int *status)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	*status = 1;
}

void	loop_export(char *arg, t_env **env_list, int *status)
{
	char	*key;
	char	*value;

	if (!is_valid_key_export(arg))
		print_error_export(arg, status);
	else if (ft_strchr(arg, '=') != NULL)
	{
		key = set_key(arg);
		value = ft_strchr(arg, '=') + 1;
		if (ft_strstr(arg, "+="))
			append_env_value(env_list, key, value);
		else
			add_env_value(env_list, key, value);
	}
	else
		add_temporary_env_value(env_list, arg);
}

int	export(t_cmd *cmd, t_env **env_list)
{
	int		i ;
	int		status;

	i = 1;
	status = 0;
	if (!cmd->args || !cmd->args[i])
		return (export_withot_args(*env_list), status);
	while (cmd->args[i])
	{
		loop_export(cmd->args[i], env_list, &status);
		i++;
	}
	return (status);
}
