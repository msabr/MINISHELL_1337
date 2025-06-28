/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:43:46 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 18:44:16 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void execuve_multypipe(t_cmd *cmds, t_env *env_list, char **envp);
{
    int i;
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t pid;

    for (i = 0; cmds[i].args != NULL; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) 
        {
            if (prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO);
            if (cmds[i].output_file)
                open_output_redirection(cmds[i].output_file);
            dup2(pipe_fd[1], STDOUT_FILENO);
            close(pipe_fd[0]);
            if (is_bultins(cmds[i].args[0]))
            {
                execute_builtin(cmds[i].args, &env_list);
            }
            else 
                execve_cmd(cmds[i].args, &env_list, envp);
        }
        else 
        {
            close(pipe_fd[1]);
            if (prev_fd != -1)
                close(prev_fd);
            prev_fd = pipe_fd[0];
        }
    }

    for (int j = 0; j < i; j++)
        wait(NULL);

    if (prev_fd != -1)
        close(prev_fd);
}
