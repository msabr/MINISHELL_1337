/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:31:05 by msabr             #+#    #+#             */
/*   Updated: 2025/07/24 19:53:45 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

void	init_execargs(t_execargs *args, t_cmd *cmds, t_env **env_list)
{
	args->n = count_cmds(cmds);
	args->pipes = NULL;
	args->env_list = env_list;
}

int	exec_multiple_pipes(t_cmd *cmds, t_env **env_list)
{
	t_execargs	args;
	pid_t		*pids;
	int			status;

	pids = NULL;
	init_execargs(&args, cmds, env_list);
	signal(SIGINT, SIG_IGN);
	if (args.n == 0)
		return (0);
	if (setup_pipes_and_pids(&args, &pids))
		return (1);
	if (fork_and_exec(cmds, pids, &args) < 0)
		return (handle_fork_error(&args));
	cleanup_and_wait(pids, &args, &status);
	return (status);
}
