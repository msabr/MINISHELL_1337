#include "minishell.h"

/*FOR COPILOT
ALWAYSE USE WHILE LOOP NOT FOR LOOP

2. Execution Model
There are two kinds of commands:

Built-in commands

Run inside the current shell process (e.g. cd, export, unset, exit)

External commands

Must use fork(), execve(), and waitpid()

🔁 Pipelines
Use pipe() to link commands

Child processes are created for each segment

Correctly duplicate file descriptors (dup2)

Close all unused fds

🔥 Redirections
>: create/truncate

>>: create/append

<: open for reading

<< (heredoc): simulate user input from inline data

🔄 Execution Flow:
Expand env vars

Parse into commands

For each command:

Setup redirections

If pipeline:

Create pipe

Fork child

Setup stdin, stdout using dup2

Execute built-in or external

Parent waits with waitpid

Restore stdio if needed

🧰 3. Builtins (MUST BE IMPLEMENTED)
Command	Run in shell	Notes
cd	Yes	Modify PWD, OLDPWD
echo	Yes	Support -n
pwd	Yes	Just getcwd()
export	Yes	Add/update env
unset	Yes	Remove env
env	Yes	Print env
exit	Yes	Exit properly, with code

➡️ Built-ins must work both with and without pipes (e.g. cd, exit won't work inside child).

🛠 4. Env Handling
Parse envp into a t_env list at startup

Implement get_env_value, set_env_value, unset_env_value

Use this list for expansions

Rebuild envp (char **) before execve()

⚠️ Execution Must Handle:
✅ Pipes ls | wc

✅ Multiple pipes cat | grep | sort

✅ Redirections ls > file, cat < file

✅ Heredocs cat << EOF

✅ Environment vars echo $HOME, $?

✅ Mixing all together cat < file | grep hi >> out

✅ Signals: handle Ctrl+C, Ctrl+D

✅ Exit status $? must be accurate

🧹 Memory & File Descriptor Management
Close fds when done

Restore stdin/stdout after redirections

Free all malloc’d memory on exit

Free command structs after execution

Avoid zombie processes → always waitpid

🚨 SIGINT/SIGQUIT Handling
Parent should catch SIGINT (Ctrl+C) and show prompt again

Children should reset to default handlers

Ctrl+D should exit cleanly

Use signal() or sigaction() properly.

🧪 Testing Plan
Test	Example
Redirection + pipe	`cat < file
Builtin + external	cd .. && ls
Heredoc edge	cat <<EOF with EOF in the middle
Var expansion	echo "$HOME"
$?	false; echo $?
Syntax errors	`ls
Signals	Ctrl+C during heredoc, prompt, or long process

✅ Final Checklist Before Push
Item	Status
Command parsing	✅
Built-in execution	✅
Env handling	✅
Pipes & redirs	✅
Heredoc	✅
Signals	✅
$? tracking	✅
No memory leaks (Valgrind)	✅
Norminette compliant	✅
Behavior matches Bash	✅

🔚 Conclusion
If the execution layer is wrong, nothing will work. Parsing is important, but execution is where most students fail:

Wrong dup2 usage

Forgetting to close pipes

Built-ins not executed in the right process

Wrong fork/exec logic

Memory leaks from bad env management

Get execution 100% solid and you win the project.*/

int g_status = 0;
void main_loop(t_env **env_list);

void	handle_exit_status(pid_t pid)
{
	int	status;
	
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		g_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		g_status = WTERMSIG(status);
		if (g_status == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
		else if (g_status == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	}
}

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

	(void)argc;
	(void)argv;
	env_list = env_to_list(envp);
	configure_environment(&env_list, envp);
	ft_handler_signal();
	main_loop(&env_list);
	return (0);
}

void main_loop(t_env **env_list)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;

	while (true)
	{
		input = ft_readline(CYAN"minishell"RED"> "RESET);
		if (!input)
			continue;
		if (input[0] != '\0')
		{
			tokens = lexer2(input);
			if (check_syntax_errors(tokens, input))
			{
				free_token_list(tokens);
				continue;
			}
			cmds = parse_tokens_to_cmds(tokens);
			handle_redirections(cmds);
			if (is_bultins(cmds->args[0]))
			{
				execve_builtin(&cmds->args[0], env_list);
			}
			else
			{
				execuve_multypipe(cmds, *env_list);
			}
			free_token_list(tokens);
		}
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
// 			if (is_bultins(cmds->args[0]))
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
