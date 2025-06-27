/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:45:34 by msabr             #+#    #+#             */
/*   Updated: 2025/06/25 20:20:22 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//obligatoire use ft_split()
// void env_to_list(t_env **env_list, char **env)
// {
//     t_env *current;
//     t_env *new_node;
//     int i;

//     *env_list = NULL;
//     i = 0;
//     while (env[i])
//     {
//         new_node = malloc(sizeof(t_env));
//         if (!new_node)
//             return; // Handle memory allocation failure
//         new_node->key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
//         new_node->value = ft_strdup(ft_strchr(env[i], '=') + 1);
//         new_node->next = NULL;
//         new_node->declared_only = true; // Assuming all env vars are exported by default

//         if (*env_list == NULL)
//             *env_list = new_node;
//         else
//         {
//             current = *env_list;
//             while (current->next)
//                 current = current->next;
//             current->next = new_node;
//         }
//         i++;
//     }
// }
