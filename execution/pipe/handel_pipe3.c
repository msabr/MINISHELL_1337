/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:43:46 by msabr             #+#    #+#             */
/*   Updated: 2025/07/14 17:40:47 by msabr            ###   ########.fr       */
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

// int	handle_exit_status(pid_t pid)
// {
// 	int	status;

// 	signal(SIGINT, SIG_IGN);
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 		status = WEXITSTATUS(status);
// 	else if (WIFSIGNALED(status))
// 	{
// 		g_status = WTERMSIG(status);
// 		if (g_status == SIGINT)
// 			ft_putstr_fd("\n", STDERR_FILENO);
// 		else if (g_status == SIGQUIT)
// 			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
// 		status = 128 + g_status;
// 	}
// 	g_status = 0;
// 	return (status);
// }

int	handle_exit_status_multiple_pipes(pid_t *pids, t_execargs args)
{
	int	status;
	int flag;
	int	i;

	status = 0;
	i = 0;
	flag = 1;
	while (i < args.n)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				g_status = WTERMSIG(status);
				if (g_status == SIGINT)
				{
					if (flag)
					{
						ft_putstr_fd("\n", STDERR_FILENO);
						flag = 0;
					}
				}
				else if (g_status == SIGQUIT)
				{
					if (flag)
					{
						ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
						flag = 0;
					}
				}
				
				status = 128 + g_status;
			}
		}
		i++;
	}
	cleanup_and_wait(pids, &args, &status);
	return (status);
}

int	exec_multiple_pipes(t_cmd *cmds, t_env **env_list)
{
	t_execargs	args;
	pid_t		*pids;

	pids = NULL;
	init_execargs(&args, cmds, env_list);
	signal(SIGINT, SIG_IGN);
	if (args.n == 0)
		return (0);
	if (setup_pipes_and_pids(&args, &pids))
		return (1);
	if (fork_and_exec(cmds, pids, &args) < 0)
		return (handle_fork_error(pids, &args));
	return (handle_exit_status_multiple_pipes(pids, args));
}
