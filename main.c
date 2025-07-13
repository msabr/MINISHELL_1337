#include "minishell.h"

int	g_status = 0;

char	*ft_readline(const char *prompt)
{
	char	*input;

	input = readline(prompt);
	if (input && *input)
	{
		add_history(input);
	}
	if (!input)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		exit(0);
	}
	return (input);
}

t_cmd	*parse_input(char *input, t_env *env_list, int *status)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = lexer(input);
    print_token_list(tokens);
	expansion_all_tokens(tokens, env_list);
    // remove_empty_token(&tokens);
	merge_collapsed_tokens(tokens);
	if (check_syntax_errors(tokens, input))
	{
		printf("syntax error\n");
		*status = 258;
		// free_token_list(tokens);
		return (NULL);
	}
	cmds = parse_tokens_to_cmd2s(tokens);
    print_cmds(cmds);
	free_token_list(tokens);
	if (!cmds)
	{
		printf("minishell: parse error\n");
		// free_cmd_list(cmds);
		return (NULL);
	}
	return (cmds);
}

// Execution phase: run commands, handle status, restore fds
void	execute_cmds(t_cmd *cmds, t_env **env_list, int *status)
{
	// print_cmds(cmds);
	save_std_fds(cmds);
	if (cmds->next)
		*status = exec_multiple_pipes(cmds, env_list);
	else
		*status = execve_simple_cmd(cmds, env_list);
	*status = handle_exit_status(*status);
	restore_std_fds(cmds);
	signal(SIGINT, handel_ctl_c);
}

void	main_loop(t_env **env_list, struct termios *saved_termios)
{
	int		status;
	char	*input;
	t_cmd	*cmds;

	status = 0;
	ft_set_status(0);
	while (true)
	{
		input = ft_readline("minishell> ");
		handel_ctl_c(g_status);
		if (input[0] != '\0')
		{
			cmds = parse_input(input, *env_list, &status);
			if (cmds)
			{
				execute_cmds(cmds, env_list, &status);
				// free_cmd_list(cmds);
			}
			// free(input);
		}
		else
			// free(input);
		tcsetattr(STDIN_FILENO, TCSANOW, saved_termios);
	}
	ft_set_status(status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env			*env_list;
	struct termios	saved_termios;

	(void)argc;
	(void)argv;
	tcgetattr(STDIN_FILENO, &saved_termios);
	configure_environment(&env_list, envp);
	ft_handler_signal();
	main_loop(&env_list, &saved_termios);
	return (0);
}
