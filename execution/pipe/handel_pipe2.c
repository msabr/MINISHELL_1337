/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:33:32 by msabr             #+#    #+#             */
/*   Updated: 2025/07/22 18:05:40 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

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

int	**create_pipes(int n)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = ft_malloc(sizeof(int *) * (n - 1));
	while (i < n - 1)
	{
		pipes[i] = ft_malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
			return (ft_perror("pipe"), NULL);
		i++;
	}
	return (pipes);
}

int	setup_pipes_and_pids(t_execargs *args, pid_t **pids)
{
	if (args->n > 1)
	{
		args->pipes = create_pipes(args->n);
		if (!args->pipes)
			return (1);
	}
	*pids = ft_malloc(sizeof(pid_t) * args->n);
	return (0);
}
