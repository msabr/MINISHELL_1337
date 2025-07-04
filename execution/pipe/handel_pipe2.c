/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:33:32 by msabr             #+#    #+#             */
/*   Updated: 2025/07/04 16:38:51 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"

void	exec_child(t_cmd *cur, t_execargs *args, int i)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	redirect_pipes(args, i);
	if (is_builtin(cur->args[0]))
	{
		execve_builtin(cur->args, args->env_list);
		exit(0);
	}
	else
	{
		path = get_path(cur->args[0], *args->env_list);
		if (!path)
		{
			ft_putstr_fd("Command not found: ", STDERR_FILENO);
			ft_putstr_fd(cur->args[0], STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			exit(127);
		}
		execve(path, cur->args, list_to_env(*args->env_list));
	}
	perror("execve");
	exit(1);
}

int	fork_and_exec(t_cmd *cmds, pid_t *pids, t_execargs *args)
{
	t_cmd	*cur;
	int		i;

	i = 0;
	cur = cmds;
	while (i < args->n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			kill_all_pids(pids, i);
			perror("fork");
			return (-1);
		}
		if (pids[i] == 0)
			exec_child(cur, args, i);
		cur = cur->next;
		i++;
	}
	return (0);
}

int	wait_all(pid_t *pids, int n)
{
	int	status;
	int	j;

	j = 0;
	status = 0;
	while (j < n)
	{
		waitpid(pids[j], &status, 0);
		j++;
	}
	return (status);
}

void	init_execargs(t_execargs *args, t_cmd *cmds, t_env **env_list)
{
	args->n = count_cmds(cmds);
	args->pipes = NULL;
	args->env_list = env_list;
}

int	setup_pipes_and_pids(t_execargs *args, pid_t **pids)
{
	if (args->n > 1)
	{
		args->pipes = create_pipes(args->n);
		if (!args->pipes)
			return (1);
	}
	*pids = malloc(sizeof(pid_t) * args->n);
	if (!*pids)
		exit(1);
	return (0);
}
