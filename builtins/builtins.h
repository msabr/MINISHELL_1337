/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:56:20 by msabr             #+#    #+#             */
/*   Updated: 2025/06/22 23:04:57 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"

bool	is_valid_key(char *key);
void	sort_env_list(t_env **env_list);
void	export_one_args(t_env **env_list);
bool	is_append(const char *arg);
void	append_env_value(t_env **env_list, const char *key, const char *value);
void	add_env_value(t_env **env_list, const char *key, const char *value);
void	export(t_cmd *cmd, t_env **env_list);

void	sort_env_list(t_env **env_list);
void	export_one_args(t_env **env_list);
void	append_env_value(t_env **env_list, const char *key, const char *value);
void	add_env_value(t_env **env_list, const char *key, const char *value);
void	add_temporary_env_value(t_env **env_list, const char *key);

#endif