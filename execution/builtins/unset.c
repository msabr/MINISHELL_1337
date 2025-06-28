/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:26 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 18:45:10 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	remove_env_variable(t_env **env_list, const char *key)
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
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (-1);
}

static int	check_env_variable(t_env **env_list, const char *key)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (0);
		current = current->next;
	}
	return (-1);
}

//unset with no options
void	unset(t_cmd *cmd, t_env **env_list)
{
	int	i;

	i = 1;
	if (is_valid_key(cmd->args[i]) == false)
	{
		ft_putstr_fd("unset: `", STDERR_FILENO);
		ft_putstr_fd(cmd->args[i], STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return ;
	}
	while (cmd->args && cmd->args[i])
	{
		if (check_env_variable(env_list, cmd->args[i]) == 0)
		{
			remove_env_variable(env_list, cmd->args[i]);
		}
		cmd->args++;
	}
}
