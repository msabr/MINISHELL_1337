#include "minishell.h"

int	g_status = 0;


t_cmd	*parse_input(char *input, t_env *env_list, int *status)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = lexer(input);
	expansion_all_tokens(tokens, env_list);
	field_split_tokens(&tokens);

	(void)env_list;
	merge_collapsed_tokens(tokens);
	if (check_syntax_errors(tokens, input))
	{
		printf("syntax error\n");
		*status = 258;
		return (NULL);
	}
	cmds = parse_tokens_to_cmd2s(tokens);
	// print_cmds(cmds);
	if (!cmds)
	{
		return (NULL);
	}
	return (cmds);
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
	main_loop(&env_list);
	return (0);
}
