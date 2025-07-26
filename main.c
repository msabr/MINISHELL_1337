#include "minishell.h"

int	g_status = 0;

char	*ft_readline(const char *prompt)
{
	char	*temp;
	char	*input;

	temp = readline(prompt);
	if (!temp)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		ft_exit(*ft_get_status());
	}
	if (temp && *temp)
		add_history(temp);
	if (g_status == SIGINT)
	{
		ft_set_status(1);
		g_status = 0;
	}
	input = ft_strdup(temp);
	free(temp);
	return (input);
}

t_cmd	*parse_input(char *input, t_env *env_list, int *status)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = lexer(input);
	// print_token_list(tokens);
	// expansion_all_tokens(tokens, env_list);
	// printf(".................apres l expension .................\n");
	// print_token_list(tokens);
	// remove_empty_token(&tokens);
	(void)env_list; // Unused parameter, can be removed if not needed
	merge_collapsed_tokens(tokens);
	if (check_syntax_errors(tokens, input))
	{
		printf("syntax error\n");
		*status = 258;
		return (NULL);
	}
	cmds = parse_tokens_to_cmd2s(tokens);
	// print_cmds(cmds);
	// free_token_list(tokens);
	if (!cmds)
	{
		printf("minishell: parse error\n");
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
	restore_std_fds(cmds);
	ft_set_status(*status);
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
		input = ft_readline("minishell$ ");
		handel_ctl_c(g_status);
		if (input[0] != '\0')
		{
			cmds = parse_input(input, *env_list, &status);
			if (cmds)
			{
				execute_cmds(cmds, env_list, &status);
			}
		}
		tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
	}
	rl_clear_history();
}
// void f(){system("leaks minishell");}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	// atexit(f);
	(void)argc;
	(void)argv;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (ft_putstr_fd("minishell: not a terminal\n", STDERR_FILENO), 1);
	configure_environment(&env_list, envp);
	ft_handler_signal();
	main_loop(&env_list);
	return (0);
}
