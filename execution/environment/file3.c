/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:16:36 by msabr             #+#    #+#             */
/*   Updated: 2025/07/06 01:10:24 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_shlvl(t_env *node, t_env ***env_list)
{
	int		shlvl_value;
	char	*new_value;

	if (!node || !node->value)
		return ;
	shlvl_value = ft_atoi(node->value);
	if (shlvl_value < 0)
	{
		ft_putstr_fd("minishell: warning: SHLVL is negative, resetting to 0\n",
			STDERR_FILENO);
		new_value = ft_itoa(0);
	}
	else
	{
		new_value = ft_itoa(shlvl_value + 1);
	}
	if (!new_value)
		return ;
	add_env_value(*env_list, "SHLVL", new_value);
}

void	configure_environment(t_env **env_list, char **env_array)
{
	t_env	*found_node;

	*env_list = env_to_list(env_array);
	found_node = find_env_node("SHLVL", *env_list);
	if (!found_node)
		add_env_value(env_list, "SHLVL", "1");
	else
		handle_shlvl(found_node, &env_list);
	found_node = find_env_node("PWD", *env_list);
	if (!found_node)
		add_env_value(env_list, "PWD", getcwd(NULL, 0));
	found_node = find_env_node("PATH", *env_list);
	if (!found_node)
		add_env_value(env_list, "PATH",
			"/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.");
	found_node = find_env_node("OLDPWD", *env_list);
	if (!found_node)
		add_temporary_env_value(env_list, "OLDPWD");
}

int	size_of_env_list(t_env *env_list)
{
	t_env	*traverser;
	int		count;

	count = 0;
	traverser = env_list;
	while (traverser)
	{
		count++;
		traverser = traverser->next;
	}
	return (count);
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

t_env	*copy_env(t_env *env_list)
{
	t_env	*new_list;
	t_env	*current;
	t_env	*new_node;

	new_list = NULL;
	current = env_list;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_strdup(current->key);
		if (current->value)
			new_node->value = ft_strdup(current->value);
		else
			new_node->value = NULL;
		new_node->export_variable = current->export_variable;
		new_node->next = new_list;
		new_list = new_node;
		current = current->next;
	}
	return (new_list);
}
