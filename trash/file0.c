/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:50:29 by msabr             #+#    #+#             */
/*   Updated: 2025/06/25 20:20:22 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environment.h"
#include "../minishell.h"

char	modify_env_variable(t_env **env_nodes, char *variable_key, char *input_value, char operation_flag);
t_env	*locate_env_node(t_env *list_head, const char *search_key);
void	add_env_node(t_env **env, char *variable_name, char *variable_value);
char	*generate_new_value(t_env *current_node, char *input_value, char append_flag);
void	deallocate_pointer(void **pointer);

void	finalize_path_setup(t_env **env, t_env *path_node, char **directory)
{
	if (!path_node)
		modify_env_variable(env, "@PATH",
			"/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.", 0);
	else
		modify_env_variable(env, "@PATH", path_node->value, 0);
	free(*directory);
}

void	setup_old_pwd(t_env **env)
{
	t_env	*existing_node;

	existing_node = locate_env_node(*env, "OLDPWD");
	if (!existing_node)
		modify_env_variable(env, "OLDPWD", NULL, 0);
}

char	*fetch_working_directory(void)
{
	char	*current_path;

	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("getcwd");
		return (NULL);
	}
	return (current_path);
}

t_env	*locate_shell_level(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "SHLVL") == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

char	*increment_shell_level(t_env *env_nodes)
{
	t_env	*level_node;
	char	*level_string;

	level_node = locate_shell_level(env_nodes);
	if (level_node)
		level_string = ft_itoa(ft_atoi(level_node->value) + 1);
	else
		level_string = ft_itoa(1);
	return (level_string);
}

void	add_env_node(t_env **env, char *variable_name, char *variable_value)
{
	t_env	*fresh_node;

	fresh_node = malloc(sizeof(t_env));
	if (!fresh_node)
		return ;
	fresh_node->key = ft_strdup(variable_name);
	fresh_node->value = ft_strdup(variable_value);
	fresh_node->next = *env;
	*env = fresh_node;
}

void	deallocate_pointer(void **pointer)
{
	if (pointer != NULL && *pointer != NULL)
	{
		free(*pointer);
		*pointer = NULL;
	}
}

char	*generate_new_value(t_env *current_node, char *input_value, char append_flag)
{
	char	*duplicate;
	char	*final_result;

	duplicate = ft_strdup(input_value);
	if (!duplicate)
		return (NULL);
	if (append_flag)
	{
		final_result = ft_strjoin(current_node->value, duplicate);
		deallocate_pointer((void **)&duplicate);
		return (final_result);
	}
	else
	{
		deallocate_pointer((void **)&current_node->value);
		return (duplicate);
	}
}

char	modify_env_variable(t_env **env_nodes, char *variable_key,
		char *input_value, char operation_flag)
{
	t_env	*iterator;
	char	*updated_val;

	iterator = *env_nodes;
	if (!env_nodes || !variable_key)
		return (-1);
	while (iterator)
	{
		if (!ft_strcmp(iterator->key, variable_key))
		{
			if (input_value != NULL)
			{
				updated_val = generate_new_value(iterator, input_value, operation_flag);
				if (!updated_val)
					return (-1);
				iterator->value = updated_val;
			}
			return (0);
		}
		iterator = iterator->next;
	}
	add_env_node(env_nodes, variable_key, input_value);
	return (0);
}

t_env	*locate_env_node(t_env *list_head, const char *search_key)
{
	t_env	*traverser;

	traverser = list_head;
	while (traverser)
	{
		if (!ft_strcmp(traverser->key, search_key))
			return (traverser);
		traverser = traverser->next;
	}
	return (NULL);
}

int	calculate_array_size(char **string_array)
{
	int	counter;

	if (!string_array)
		return (0);
	counter = 0;
	while (string_array[counter])
		counter++;
	return (counter);
}

void env_to_list(t_env **env_list, char **env)
{
    t_env *current;
    t_env *new_node;
    int i;

    *env_list = NULL;
    i = 0;
    while (env[i])
    {
        new_node = malloc(sizeof(t_env));
        if (!new_node)
            return; 
        new_node->key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
        new_node->value = ft_strdup(ft_strchr(env[i], '=') + 1);
        new_node->next = NULL;
        new_node->declared_only = true; 
        if (*env_list == NULL)
            *env_list = new_node;
        else
        {
            current = *env_list;
            while (current->next)
                current = current->next;
            current->next = new_node;
        }
        i++;
    }
}

void	configure_environment1(t_env **env_list, char **env_array)
{
	t_env	*found_node;
	char	*temp_value;

	temp_value = NULL;
	env_to_list(env_list, env_array);
	found_node = locate_env_node(*env_list, "SHLVL");
	if (!found_node)
		modify_env_variable(env_list, "SHLVL", "1", 0);
	else
	{
		temp_value = increment_shell_level(*env_list);
		modify_env_variable(env_list, "SHLVL", temp_value, 0);
		free(temp_value);
		temp_value = NULL;
	}
	found_node = locate_env_node(*env_list, "PWD");
	temp_value = fetch_working_directory();
	if (!found_node)
		modify_env_variable(env_list, "PWD", temp_value, 0);
	modify_env_variable(env_list, "@PWD", temp_value, 0);
	found_node = locate_env_node(*env_list, "PATH");
	finalize_path_setup(env_list, found_node, &temp_value);
	setup_old_pwd(env_list);
}
