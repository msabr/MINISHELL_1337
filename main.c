/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:39:26 by msabr             #+#    #+#             */
/*   Updated: 2025/06/26 15:24:57 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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



void execuve_multypipe(t_cmd *cmds, t_env *env_list, char **envp);

//withot parrsing
int	main(int argc, char **argv, char **env)
{
	t_env *env_list;
	// t_cmd *cmds;
	
	
	configure_environment(&env_list, env);
	if (argc > 1 || argv[1])
	{
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
		return (1);
	}
	while (true)
	{
		char *input = readline(CYAN"minishell"RED"> "RESET);
		if (!input)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			char **args = ft_split_space(input);
			if (is_bultins(args[0]))
			{
				execve_builtin(args, &env_list);
			}
			else
			{
				char *path = get_path(args[0], env_list);
				if (!path)
				{
					ft_putstr_fd("Command not found: ", STDERR_FILENO);
					ft_putstr_fd(args[0], STDERR_FILENO);
					ft_putchar_fd('\n', STDERR_FILENO);
					free_split(args);
					free(input);
					continue;
				}
				pid_t pid = fork();
				if (pid < 0)
				{
					perror("fork");
				}
				else if (pid == 0)
				{
					execve(path, args, env);
					perror("execve");
					exit(EXIT_FAILURE);
				}
				else
				{
					int status;
					waitpid(pid, &status, 0);
				}
			}
			free_split(args);
		}	
	}
	return (0);
}