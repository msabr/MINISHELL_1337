/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 23:13:27 by msabr             #+#    #+#             */
/*   Updated: 2025/07/03 23:54:05 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_is_dir(const char *path)
{
	int	fd;
	
	fd = open(path, O_RDONLY | O_DIRECTORY);
	if (fd < 0)
	{
		return (false);
	}
	close(fd);
	return (true);
}

int execve_simple_cmd(t_cmd *cmds, t_env **env_list)
{
	int		status;
	char	*path;
	pid_t	pid;
	
	status = 0;
	if (is_builtin(cmds->args[0]))
	{
		execve_builtin(cmds->args, env_list);
		return (0);
	}
	if (ft_strchr(cmds->args[0], '/'))
	{
		if (!ft_is_dir(cmds->args[0]))
			path = cmds->args[0];
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			if (ft_is_dir(cmds->args[0]))
			{
				ft_putstr_fd(cmds->args[0], STDERR_FILENO);
				ft_putstr_fd(": is a directory\n", STDERR_FILENO);
			}
			else
			{
				access(cmds->args[0], X_OK);
				perror(cmds->args[0]);
			}
			return (1);
		}
	}
	else
	{
		path = get_path(cmds->args[0], *env_list);
		if (!path)
		{
			ft_putstr_fd("Command not found: ", STDERR_FILENO);
			ft_putstr_fd(cmds->args[0], STDERR_FILENO);
			ft_putchar_fd('\n', STDERR_FILENO);
			return (127);
		}
	}
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (is_builtin(cmds->args[0]))
			execve_builtin(cmds->args, env_list);
		else
			execve(path, cmds->args, list_to_env(*env_list));
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
		status = handle_exit_status(pid);
	return (status);
	
	
}


void tt(void)
{
	write(1, "\033[32m----------------------\033[0m\n\n", 32);
}


char *get_path(char *cmd, t_env *env_list)
{
	char	*path_env;
	t_env	*current;
	// char	**paths;
	// char	*full_path;

	path_env = NULL;
	current = env_list;
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
		return cmd;
	char **paths = ft_split(path_env, ':');
	char *full_path = NULL;
	for (int i = 0; paths[i]; i++)
	{
		full_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(full_path, cmd);
		if (!ft_is_dir(full_path) && access(full_path, X_OK | F_OK) == 0)
		{
			return full_path;
		}
	}
	return NULL;
}

//withot execve_simple_cmd
// pipe: fd[2] → fd[0]=read, fd[1]=write
// fork L: dup2(fd[1], 1), close fd, exec(cmd1)
// fork R: dup2(fd[0], 0), close fd, exec(cmd2)
// parent: close fd, waitpid()

// N pipes = N+1 procs
// pipe[i] connects proc[i] → proc[i+1]
// last cmd reads from pipe[N-1][0], first writes to pipe[0][1]

// block: writer waits if reader slow
// child: close unused pipe ends

// setup pipes[i]: pipe(pipes[i])
// before fork[i]:
//   if i > 0 → dup2(pipes[i-1][0], 0)
//   if i < N-1 → dup2(pipes[i][1], 1)
// close all pipes in parent

// execve(): no shell, raw syscall

static int count_cmds(t_cmd *cmds)
{
    int count = 0;
    while (cmds)
    {
        count++;
        cmds = cmds->next;
    }
    return count;
}

int exec_multiple_pipes(t_cmd *cmds, t_env **env_list)
{
    int num_cmds = count_cmds(cmds);
    int **pipes = NULL;
    pid_t *pids = NULL;
    int status = 0;

	signal(SIGINT, SIG_IGN);
    if (num_cmds == 0)
        return (0);

    // Allocate pipes
    if (num_cmds > 1)
    {
        pipes = malloc(sizeof(int *) * (num_cmds - 1));
        if (!pipes)
            return (perror("malloc"), 1);
        for (int i = 0; i < num_cmds - 1; i++)
        {
            pipes[i] = malloc(sizeof(int) * 2);
            if (!pipes[i])
                return (perror("malloc"), 1);
            if (pipe(pipes[i]) == -1)
                return (perror("pipe"), 1);
        }
    }

    pids = malloc(sizeof(pid_t) * num_cmds);
    if (!pids)
        exit(1);
    t_cmd *cur = cmds;
    for (int i = 0; i < num_cmds; i++)
    {
        pids[i] = fork();
        if (pids[i] < 0)
		{
			num_cmds = i;
			while (i > 0)
			{
				kill(pids[i], SIGKILL);
				// waitpid(pids[i], NULL, 0);
			}
			break ;
		}
        if (pids[i] == 0)
        {
		signal(SIGINT, SIG_DFL);
            // If not the first command, redirect stdin
            if (i > 0)
                dup2(pipes[i - 1][0], STDIN_FILENO);
            // If not the last command, redirect stdout
            if (i < num_cmds - 1)
                dup2(pipes[i][1], STDOUT_FILENO);

            // Close all pipe fds
            for (int j = 0; j < num_cmds - 1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            // Execute
            if (is_builtin(cur->args[0]))
			{
                execve_builtin(cur->args, env_list);
				exit(0); // Exit if it's a builtin
			}
            else
            {
				char *path = get_path(cur->args[0], *env_list);
				if (!path)
				{
					ft_putstr_fd("Command not found: ", STDERR_FILENO);
					ft_putstr_fd(cur->args[0], STDERR_FILENO);
					ft_putchar_fd('\n', STDERR_FILENO);
					exit(127);
				}
				execve(path, cur->args, list_to_env(*env_list));
			}
            perror("execve");
            exit(1);
        }
        cur = cur->next;
    }
    // Parent closes all pipes
    for (int i = 0; i < num_cmds - 1; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        free(pipes[i]);
    }
    free(pipes);
    for (int j = 0; j < num_cmds; j++)
        waitpid(pids[j], &status, 0);
    free(pids);

    return (status);
}
