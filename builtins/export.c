/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:06 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 22:57:51 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	is_append(const char *arg)
{
	return (ft_strstr(arg, "+=") != NULL);
}

void	print_error_export(char *arg)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

char	*set_key(const char *arg)
{
	char	*key;
	size_t	len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	key = malloc(len + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, arg, len + 1);
	return (key);
}

void	export(t_cmd *cmd, t_env **env_list)
{
	int		i ;
	char	*key;
	char	*value;

	i = 1;
	if (!cmd->args || !cmd->args[i])
		export_one_args(env_list);
	if (!is_valid_key(cmd->args[i]))
		return (print_error_export(cmd->args[i]));
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '=') != NULL)
		{
			key = set_key(cmd->args[i]);
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
