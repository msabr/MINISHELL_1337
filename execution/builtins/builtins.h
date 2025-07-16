/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:18:31 by msabr             #+#    #+#             */
/*   Updated: 2025/07/15 17:57:52 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../../minishell.h"

bool	is_valid_key_export(char *key);
bool	is_valid_key_unset(char *key);
void	sort_env_list(t_env **env_list);
void	append_env_value(t_env **env_list, char *key, char *value);
void	export_withot_args(t_env *env_list);

#endif