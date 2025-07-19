/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:48:57 by msabr             #+#    #+#             */
/*   Updated: 2025/07/19 15:49:56 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipe.h"



void	exec_child(t_cmd *cur, t_execargs *args, int i)
{
	char	*path;
	int		status;

	set_default_signals();
	redirect_pipes(args, i);
	cur->in_pipe = true;
	if (is_redirection(cur))
		if (!handle_redirections(cur, *args->env_list))
			ft_set_and_exit(1);
	if (!cur || !cur->args || !cur->args[0])
		ft_set_and_exit(0);
	if (is_builtin(cur->args[0]))
		ft_set_and_exit(execve_builtin(cur, args->env_list));
	else
	{
		status = get_exec_path(cur, args->env_list, &path);
		if (status)
			ft_set_and_exit(status);
		execve(path, cur->args, list_to_env(*args->env_list));
	}
	perror("minishell");
	ft_set_and_exit(1);
}

int	handle_fork_error(pid_t *pids, t_execargs *args)
{
	free(pids);
	if (args->pipes)
		close_pipes(args->pipes, args->n);
	return (1);
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
