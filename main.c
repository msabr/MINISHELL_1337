/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 10:23:13 by msabr             #+#    #+#             */
/*   Updated: 2025/08/02 01:21:04 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void)argc;
	(void)argv;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (1);
	printbanner();
	configure_environment(&env_list, envp);
	main_loop(&env_list);
	return (0);
}
