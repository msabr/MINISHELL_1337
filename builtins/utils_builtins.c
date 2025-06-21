/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:54:59 by msabr             #+#    #+#             */
/*   Updated: 2025/06/20 21:04:22 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

//get_env_value
char *get_env_value(t_env *env_list, const char *key)
{
    t_env *current = env_list;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return (current->value);
        current = current->next;
    }
    return (NULL);
}

//set_env_value
void set_env_value(t_env *env_list, const char *key, const char *value)
{
    t_env *current = env_list;

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

    // If the key does not exist, create a new entry
    t_env *new_entry = malloc(sizeof(t_env));
    new_entry->key = ft_strdup(key);
    new_entry->value = ft_strdup(value);
    new_entry->next = env_list;
    env_list = new_entry;
}
//free_env_list
void free_env_list(t_env *env_list)
{
    t_env *current = env_list;
    t_env *next;

    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}

//remove_env_variable
int remove_env_variable(t_env *env_list, const char *key)
{
    t_env *current = env_list;
    t_env *prev = NULL;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                env_list = current->next; // Update head if removing first element
            free(current->key);
            free(current->value);
            free(current);
            return 0; // Success
        }
        prev = current;
        current = current->next;
    }
    return -1; // Not found
}

int check_env_variable(t_env *env_list, const char *key)
{
    t_env *current = env_list;

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return 1; // Variable exists
        current = current->next;
    }
    return 0; // Variable does not exist
}

//add_env_variabl
// void add_env_variable(t_env **env_list, const char *key, const char *value, int is_exported)
// {
//     t_env *new_entry = malloc(sizeof(t_env));
//     new_entry->key = ft_strdup(key);
//     new_entry->value = value ? ft_strdup(value) : NULL;
//     // new_entry->is_exported = is_exported;
//     new_entry->next = *env_list;
//     *env_list = new_entry;
// }
