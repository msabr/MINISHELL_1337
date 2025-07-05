#include "minishell.h"
// void f(void)
// {
// 	system("leaks minishell");
// }
int g_status = 0;

char *ft_readline(const char *prompt)
{
	char *input;

	input = readline(prompt);
	if (input && *input)
	{
		add_history(input);
	}
	return (input);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;
	struct termios	saved_termios;

	// atexit(f);
	(void)argc;
	(void)argv;
	tcgetattr(STDIN_FILENO, &saved_termios);
	configure_environment(&env_list, envp);
	ft_handler_signal();
	main_loop(&env_list, &saved_termios);
	return (0);
}

void main_loop(t_env **env_list, struct termios *saved_termios)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	status = 0;	
	while (true)
	{
		input = ft_readline("minishell> ");
		if (g_status == SIGINT)
		{
			status = 1;
			g_status = 0;
		}
		if (!input)
			return (ft_putstr_fd("exit\n", STDERR_FILENO));
		if (input[0] != '\0')
		{
			tokens = lexer2(input);
			// print_token_list(tokens);
			if (check_syntax_errors(tokens, input))
			{
				status = 258;
				free_token_list(tokens);
				// printf("Exit status: %d\n", status);
				continue;
			}
			cmds = parse_tokens_to_cmds(tokens);
			// print_cmds(cmds);

			if (!cmds)
			{
				free_token_list(tokens);
				// printf("Exit status: %d\n", status);
				continue;
			}
			int in_fd, out_fd;
			in_fd = dup(STDIN_FILENO);
			out_fd = dup(STDOUT_FILENO);
			// printf("Executing command: %s\n", cmds->next->args[0]);
			if (cmds->next)
				status = exec_multiple_pipes(cmds, env_list);
			else
			{
				// puts("Executing single command");
				status = execve_simple_cmd(cmds, env_list);
			}
			status = handle_exit_status(status);
			dup2(in_fd, STDIN_FILENO);
			dup2(out_fd, STDOUT_FILENO);
			close(in_fd);
			close(out_fd);
			signal(SIGINT, sig_ctl_c);
		}
		tcsetattr(STDIN_FILENO, TCSANOW, saved_termios);
		// printf("Exit status: %d\n", status);
	}
}
