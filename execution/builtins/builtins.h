/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:18:31 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 12:57:24 by msabr            ###   ########.fr       */
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
void	check_flags(t_cmd *cmd, int *i, int *newline);
void	env_function(t_env *env_list);
int		get_sign(const char *str, int *i);
bool	valid_number(const char *str);
int		exit_shell(t_cmd *cmd);
void	swap_env_nodes(t_env *a, t_env *b);
void	print_export_variable(t_env *current);
void	print_error_export(char *arg, int *status);
int		export(t_cmd *cmd, t_env **env_list);
void	pwd(t_env **env_list);
int		check_env_variable(t_env **env_list, const char *key);
int		unset(t_cmd *cmd, t_env **env_list);

#endif