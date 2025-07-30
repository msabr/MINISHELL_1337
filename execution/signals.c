/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 08:43:30 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 10:36:10 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	handel_ctl_c(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = SIGINT;
	}
}

void	ft_handler_signal(void)
{
	g_status = 0;
	rl_catch_signals = 0;
	signal(SIGINT, handel_ctl_c);
	signal(SIGQUIT, SIG_IGN);
}

int	handle_exit_status(pid_t pid, int status)
{
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_status = WTERMSIG(status);
		if (g_status == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if (g_status == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		status = 128 + g_status;
	}
	g_status = 0;
	return (status);
}

void	set_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
