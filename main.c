/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 02:39:26 by msabr             #+#    #+#             */
/*   Updated: 2025/06/14 14:27:43 by msabr            ###   ########.fr       */
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
			
			
			if (args && args[0])
			{
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
