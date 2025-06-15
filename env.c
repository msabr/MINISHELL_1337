/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:45:34 by msabr             #+#    #+#             */
/*   Updated: 2025/06/14 11:52:40 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void env_to_list(t_env **env_list, char **env)
{
    t_env *new_node;
    t_env *current;

    *env_list = NULL;
    current = NULL;

    for (int i = 0; env[i]; i++)
    {
        new_node = malloc(sizeof(t_env));
        if (!new_node)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        
        new_node->key = ft_strdup(ft_strtok(env[i], "="));
        new_node->value = ft_strdup(ft_strtok(NULL, "="));
        new_node->next = NULL;

        if (*env_list == NULL)
            *env_list = new_node;
        else
            current->next = new_node;

        current = new_node;
    }
}