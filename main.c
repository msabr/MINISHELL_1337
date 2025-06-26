/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:39:26 by msabr             #+#    #+#             */
/*   Updated: 2025/06/15 16:02:53 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_path(char *cmd, t_env *env_list)
{
	char *path_env = NULL;
	t_env *current = env_list;

	while (current)
	{
		if (ft_strncmp(current->key, "PATH", 4) == 0)
		{
			path_env = current->value;
			break;
		}
		current = current->next;
	}

	if (!path_env)
		return cmd; // If no PATH variable, return the command as is

	char **paths = ft_split(path_env, ':');
	char *full_path = NULL;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return cmd; // If command is an absolute or relative path and executable
		else
			return NULL; // If not executable, return NULL
	}

	for (int i = 0; paths[i]; i++)
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return full_path; // Return the full path if executable
		}
		free(full_path);
	}
	free_split(paths);
	return NULL; // Return the original command if not found
}
 
int is_bultins(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "exit") == 0 ||
		ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0)
	{
		return 1; // Command is a builtin
	}
	return 0; // Command is not a builtin

}
void execve_builtin(char **args, t_env **env_list)
{
	t_cmd cmd;
	cmd.args = args;
	if(ft_strcmp(args[0], "echo") == 0)
		echo(&cmd);
	else if(ft_strcmp(args[0], "pwd") == 0)
		pwd();
	else if(ft_strcmp(args[0], "cd") == 0)
		cd(&cmd, *env_list);
	else if(ft_strcmp(args[0], "exit") == 0)
		exit_shell(&cmd, *env_list);
	else if(ft_strcmp(args[0], "env") == 0)
		env_function(*env_list);
	else if(ft_strcmp(args[0], "export") == 0)
		export(&cmd, *env_list);
	else if(ft_strcmp(args[0], "unset") == 0)
		unset(&cmd, *env_list);
	else
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);

}


int	main(int argc, char **argv, char **env)
{
	t_env *env_list;
	
	env_to_list(&env_list, env);
	if (argc > 1 || argv[1])
	{
		ft_putstr_fd("Usage: ./minishell\n", STDERR_FILENO);
		return (1);
	}
	while (true)
	{
		char *input = readline("minishell> ");
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
