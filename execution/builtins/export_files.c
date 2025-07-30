/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_files.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 22:47:00 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 12:54:27 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	swap_env_nodes(t_env *a, t_env *b)
{
	t_env	*temp;

	temp = ft_malloc(sizeof(t_env));
	temp->key = a->key;
	temp->value = a->value;
	temp->export_variable = a->export_variable;
	a->key = b->key;
	a->value = b->value;
	a->export_variable = b->export_variable;
	b->key = temp->key;
	b->value = temp->value;
	b->export_variable = temp->export_variable;
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

void	print_export_variable(t_env *current)
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

void	export_withot_args(t_env *env_list)
{
	t_env	*current;

	current = copy_env(env_list);
	sort_env_list(&current);
	while (current)
	{
		if (ft_strcmp(current->key, "_") && ft_strcmp(current->key, "1PWD"))
			print_export_variable(current);
		current = current->next;
	}
}
