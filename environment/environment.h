/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 16:53:39 by msabr             #+#    #+#             */
/*   Updated: 2025/06/25 20:28:44 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H      

# include "../Libft/libft.h"
typedef struct s_env   t_env;

t_env	*locate_env_node(t_env *list_head, const char *search_key);

void	configure_environment(t_env **env, char **env_array);

#endif