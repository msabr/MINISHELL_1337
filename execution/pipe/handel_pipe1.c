/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:31:05 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 16:38:41 by msabr            ###   ########.fr       */
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

int	**create_pipes(int n)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = malloc(sizeof(int *) * (n - 1));
	if (!pipes)
		return (perror("malloc"), NULL);
	while (i < n - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (perror("malloc"), NULL);
		if (pipe(pipes[i]) == -1)
			return (perror("pipe"), NULL);
		i++;
	}
	return (pipes);
}

void	close_and_free_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	kill_all_pids(pid_t *pids, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		kill(pids[i], SIGKILL);
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

void	redirect_pipes(t_execargs *args, int i)
{
	int	j;

	j = 0;
	if (i > 0)
		dup2(args->pipes[i - 1][0], STDIN_FILENO);
	if (i < args->n - 1)
		dup2(args->pipes[i][1], STDOUT_FILENO);
	while (j < args->n - 1)
	{
		close(args->pipes[j][0]);
		close(args->pipes[j][1]);
		j++;
	}
}
