/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:56:20 by msabr             #+#    #+#             */
/*   Updated: 2025/06/15 16:03:28 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

char *get_env_value(t_env *env_list, const char *key);
void set_env_value(t_env *env_list, const char *key, const char *value);
void free_env_list(t_env *env_list);
int remove_env_variable(t_env *env_list, const char *key);
int check_env_variable(t_env *env_list, const char *key);




#endif