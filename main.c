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
    char    *input;
    t_token *tokens;
    t_cmd   *cmds;
    int     status;

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
            expand_token_list(tokens, env_list, g_status, 0);

            if (check_syntax_errors(tokens, input))
            {
                status = 258;
                free_token_list(tokens);
                printf("Exit status: %d\n", status);
                free(input);
                continue;
            }
            cmds = parse_tokens_to_cmds(tokens);
            // print_cmds(cmds);
            // Gestion de l'affectation locale var=3
            if (cmds && cmds->args && cmds->args[0] && is_assignment(cmds->args[0]))
            {
                char *eq = ft_strchr(cmds->args[0], '=');
                if (eq)
                {
                    *eq = '\0';
                    set_env_value(env_list, cmds->args[0], eq + 1);
                    *eq = '=';
                }
                status = 0;
            }
            else if (cmds)
            { 
                if (is_redirection(cmds))
                {
                    int saved_stdin = dup(STDIN_FILENO);
                    int saved_stdout = dup(STDOUT_FILENO);
                    if (!handle_redirections(cmds))
                    {
                        printf("Exit status: %d\n", 1);
                        dup2(saved_stdin, STDIN_FILENO);
                        dup2(saved_stdout, STDOUT_FILENO);
                        close(saved_stdin);
                        close(saved_stdout);
                        free_token_list(tokens);
                        // free_cmds(cmds);
                        free(input);
                        continue;
                    }
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin);
                    close(saved_stdout);
                }
                if (cmds->next)
                {
                    int in_fd = dup(STDIN_FILENO);
                    int out_fd = dup(STDOUT_FILENO);
                    status = exec_multiple_pipes(cmds, env_list);
                    dup2(in_fd, STDIN_FILENO);
                    dup2(out_fd, STDOUT_FILENO);
                    close(in_fd);
                    close(out_fd);
                    status = handle_exit_status(status);
                }
                else
                {
                    status = execve_simple_cmd(cmds, env_list);
                }
                signal(SIGINT, sig_ctl_c);
            }
            free_token_list(tokens);
        }
        tcsetattr(STDIN_FILENO, TCSANOW, saved_termios);
        printf("Exit status: %d\n", status);
        free(input);
    }
}
//withot parrsing


// void main_loop(t_env **env_list)
// {
// 	char *input;
// 	t_token *tokens;
// 	t_cmd *cmds;

// 	while (true)
// 	{
// 		input = readline(CYAN"minishell"RED"> "RESET);
// 		if (!input)
// 		{
// 			ft_putstr_fd("exit\n", STDERR_FILENO);
// 			break;
// 		}
// 		if (input[0] != '\0')
// 		{
// 			add_history(input);
// 			tokens = lexer2(input);
// 			print_token_list(tokens);
// 			if (check_syntax_errors(tokens, input))
// 			{
// 				free_token_list(tokens);
// 				continue;
// 			}
// 			cmds = parse_tokens_to_cmds(tokens);
// 			print_cmds(cmds);
// 			if (is_builtin(cmds->args[0]))
// 			{
// 				execve_builtin(&cmds->args[0], env_list);
// 			}
// 			else if (!ft_strcmp(cmds->args[0], "/"))
// 			{
// 				ft_putendl_fd("minishell: /: is a directory", STDERR_FILENO);
// 			}
// 			else
// 			{
// 				char *path = get_path(cmds->args[0], *env_list);
// 				if (!path)
// 				{
// 					ft_putstr_fd("Command not found: ", STDERR_FILENO);
// 					ft_putstr_fd(cmds->args[0], STDERR_FILENO);
// 					ft_putchar_fd('\n', STDERR_FILENO);
// 					continue;
// 				}
// 				signal(SIGINT, SIG_IGN);
// 				pid_t pid = fork();
// 				if (pid < 0)
// 				{
// 					perror("fork");
// 				}
// 				else if (pid == 0)
// 				{
// 					signal(SIGINT, SIG_DFL);
// 					signal(SIGQUIT, SIG_DFL);
// 					execve(path, cmds->args, list_to_env(*env_list));
// 					perror("execve");
// 					exit(EXIT_FAILURE);
// 				}
// 				else
// 				{
// 					int status;
// 					waitpid(pid, &status, 0);
// 					if (WIFEXITED(status))
// 					{
// 						g_status = WEXITSTATUS(status);
// 					}
// 					else if (WIFSIGNALED(status))
// 					{
// 						g_status = WTERMSIG(status);
// 						if (g_status == SIGINT)
// 							ft_putstr_fd("\n", STDERR_FILENO);
// 						else if (g_status == SIGQUIT)
// 							ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
// 					}		
// 				}
// 			}
			
// 		}
// 	}
// }
