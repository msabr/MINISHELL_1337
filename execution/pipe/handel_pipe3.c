/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:43:46 by msabr             #+#    #+#             */
/*   Updated: 2025/07/16 16:06:29 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

int	count_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

void	close_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	cleanup_and_wait(pid_t *pids, t_execargs *args, int *status)
{
	if (args->pipes)
		close_pipes(args->pipes, args->n);
	*status = wait_all(pids, args->n);
	ft_set_status(*status);
}

void	print_signal(int sig, int *flag)
{
	if (*flag)
	{
		if (sig == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if (sig == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		*flag = 0;
	}
}

int	wait_all(pid_t *pids, int n)
{
	int	last_status;
	int	flag;
	int	i;
	int	s;

	last_status = 0;
	flag = 1;
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &s, 0);
		if (WIFSIGNALED(s))
			print_signal(WTERMSIG(s), &flag);
		if (i == n - 1)
			last_status = s;
		i++;
	}
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (last_status);
}
