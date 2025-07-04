/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:43:46 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 16:38:57 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

int	handle_fork_error(pid_t *pids, t_execargs *args)
{
	free(pids);
	if (args->pipes)
		close_and_free_pipes(args->pipes, args->n);
	return (1);
}

void	cleanup_and_wait(pid_t *pids, t_execargs *args, int *status)
{
	if (args->pipes)
		close_and_free_pipes(args->pipes, args->n);
	*status = wait_all(pids, args->n);
	free(pids);
}

int	exec_multiple_pipes(t_cmd *cmds, t_env **env_list)
{
	t_execargs	args;
	pid_t		*pids;
	int			status;

	status = 0;
	pids = NULL;
	init_execargs(&args, cmds, env_list);
	signal(SIGINT, SIG_IGN);
	if (args.n == 0)
		return (0);
	if (setup_pipes_and_pids(&args, &pids))
		return (1);
	if (fork_and_exec(cmds, pids, &args) < 0)
		return (handle_fork_error(pids, &args));
	cleanup_and_wait(pids, &args, &status);
	return (status);
}
