/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:18:31 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 10:40:09 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../execution.h"

bool	is_valid_key_export(char *key);
bool	is_valid_key_unset(char *key);
void	sort_env_list(t_env **env_list);
void	append_env_value(t_env **env_list, char *key, char *value);
void	export_withot_args(t_env *env_list);
int		cd(t_cmd *cmd, t_env **env_list);
void	echo(t_cmd *cmd);
void	env_function(t_env *env_list);
int		exit_shell(t_cmd *cmd);
int		export(t_cmd *cmd, t_env **env_list);
void	pwd(t_env **env_list);
int		unset(t_cmd *cmd, t_env **env_list);

#endif