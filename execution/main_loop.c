/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:52:45 by msabr             #+#    #+#             */
/*   Updated: 2025/07/29 17:50:21 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*ft_readline(const char *prompt)
{
	char	*temp;
	char	*input;

	temp = readline(prompt);
	if (g_status == SIGINT)
	{
		ft_set_status(1);
		g_status = 0;
	}
	if (!temp)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_exit(*ft_get_status());
	}
	if (temp && *temp)
		add_history(temp);
	input = ft_strdup(temp);
	free(temp);
	return (input);
}

bool	is_redirection2(t_cmd *cmds)
{
	t_redir		*current;

	current = cmds->redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN)
			return (true);
		current = current->next;
	}
	return (false);
}

void	execute_cmds(t_cmd *cmds, t_env **env_list, int *status)
{
	// print_cmds(cmds);
	save_std_fds(cmds);
	if (is_heredoc(cmds))
	{
		if (handel_heredoc(cmds, *env_list))
			ft_set_status(0);
		else
		{
			restore_std_fds(cmds);
			ft_set_status(1);
			return ;
		}
	}
	if (cmds->next)
	{
		*status = exec_multiple_pipes(cmds, env_list);
		ft_set_status(*status);
	}
	else if (cmds->args || (!cmds->args && is_redirection2(cmds)))
	{
		*status = execve_simple_cmd(cmds, env_list);
		ft_set_status(*status);
	}
	restore_std_fds(cmds);
	signal(SIGINT, handel_ctl_c);
}

void	main_loop(t_env **env_list)
{
	int				status;
	char			*input;
	t_cmd			*cmds;
	struct termios	saved_termios;

	status = *ft_get_status();
	tcgetattr(STDIN_FILENO, &saved_termios);
	while (true)
	{
		ft_handler_signal();
		input = ft_readline("minishell$ ");
		handel_ctl_c(g_status);
		if (input[0] != '\0')
		{
			cmds = parse_input(input, *env_list, &status);
			if (cmds)
				execute_cmds(cmds, env_list, &status);
		}
		close_heredoc(cmds);
		tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
	}
	rl_clear_history();
}
