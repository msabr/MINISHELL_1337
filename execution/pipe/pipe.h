/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:32:29 by msabr             #+#    #+#             */
/*   Updated: 2025/07/31 17:38:29 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H

# include "../execution.h"

typedef struct s_execargs
{
	int		n;
	int		**pipes;
	t_env	**env_list;
}	t_execargs;

void	init_execargs(t_execargs *args, t_cmd *cmds, t_env **env_list);
int		exec_multiple_pipes(t_cmd *cmds, t_env **env_list);
void	redirect_pipes(t_execargs *args, int i);
int		**create_pipes(int n);
int		setup_pipes_and_pids(t_execargs *args, pid_t **pids);
int		count_cmds(t_cmd *cmds);
void	close_pipes(int **pipes, int n);
void	cleanup_and_wait(pid_t *pids, t_execargs *args, int *status);
void	print_signal(int sig, int *flag);
int		wait_all(pid_t *pids, int n);
void	exec_child(t_cmd *cur, t_execargs *args, int i);
int		handle_fork_error(t_execargs *args);
void	kill_all_pids(pid_t *pids, int n);
int		fork_and_exec(t_cmd *cmds, pid_t *pids, t_execargs *args);

#endif