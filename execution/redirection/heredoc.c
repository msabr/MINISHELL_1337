// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   heredoc.c                                          :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/06/23 18:09:58 by msabr             #+#    #+#             */
// /*   Updated: 2025/07/16 13:38:23 by msabr            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../minishell.h"

// // static int	parent_heredoc(t_heredoc *here, pid_t pid)
// // {
// // 	int	status;
// // 	int	exit_code;

// // 	close(here->fd_write);
// // 	waitpid(pid, &status, 0);
// // 	if (WIFSIGNALED(status))
// // 	{
// // 		close(here->fd_read);
// // 		set_exit_status(1);
// // 		return (1);
// // 	}
// // 	else if (WIFEXITED(status))
// // 	{
// // 		exit_code = WEXITSTATUS(status);
// // 		if (exit_code == 130 || exit_code != 0)
// // 		{
// // 			close(here->fd_read);
// // 			set_exit_status(1);
// // 			return (1);
// // 		}
// // 	}
// // 	return (0);
// // }

// // static int	child_heredoc(t_heredoc *here)
// // {
// // 	pid_t	pid;

// // 	pid = fork();
// // 	if (pid == 0)
// // 	{
// // 		close(here->fd_read);
// // 		handle_signals();
// // 		here_dic_helper(here);
// // 	}
// // 	else if (pid < 0)
// // 	{
// // 		perror("fork");
// // 		close(here->fd_write);
// // 		close(here->fd_read);
// // 		return (-1);
// // 	}
// // 	return (pid);
// // }

// // static int	open_heredoc(t_heredoc *here)
// // {
// // 	unlink("/tmp/heredoc_temp");
// // 	here->fd_write = open("/tmp/heredoc_temp", O_WRONLY
// // 			| O_CREAT | O_TRUNC, 0666);
// // 	here->fd_read = open("/tmp/heredoc_temp", O_RDONLY);
// // 	if (here->fd_write < 0 || here->fd_read < 0)
// // 	{
// // 		perror("open");
// // 		close(here->fd_write);
// // 		close(here->fd_read);
// // 		return (1);
// // 	}
// // 	unlink("/tmp/heredoc_temp");
// // 	return (0);
// // }

// // static int	heredoc_utils(t_heredoc *here)
// // {
// // 	pid_t	pid;

// // 	if (open_heredoc(here) != 0)
// // 		return (1);
// // 	pid = child_heredoc(here);
// // 	if (pid == -1)
// // 		return (1);
// // 	if (parent_heredoc(here, pid) != 0)
// // 		return (1);
// // 	if (here->index + 1 == here->cmd->heredoc_num)
// // 		here->cmd->fd_heredoc = here->fd_read;
// // 	else
// // 		close(here->fd_read);
// // 	return (0);
// // }

// // void	handle_heredoc_sig(int sig)
// // {
// // 	(void)sig;
// // 	write(1, "\n", 1);
// // 	exit(1);
// // }

// // void	handle_signals(void)
// // {
// // 	signal(SIGINT, handle_heredoc_sig);
// // 	signal(SIGQUIT, SIG_IGN);
// // }

// // int	here_doc(t_cmd *cmd, t_env **env)
// // {
// // 	t_heredoc	    tmp;

// // 	tmp = cmd->heredocs;
// // 	while (tmp)
// // 	{
// // 		tmp.index = 0;
// // 		tmp.env = env;
// // 		while (tmp.index < tmp->heredoc_num)
// // 		{
// // 			if (heredoc_utils(&tmp) != 0)
// // 				return (1);
// // 			tmp.index++;
// // 		}
// // 		tmp = tmp->next;
// // 	}
// // 	return (0);
// // }
// void	write_heredoc(int fd_write, char *str, t_env **env, int flag)
// {
// 	char	*exp_str;

// 	if (flag)
// 	{
// 		write(fd_write, str, ft_strlen(str));
// 		write(fd_write, "\n", 1);
// 	}
// 	else
// 	{
// 		exp_str = expand_token(str, env);
// 		write(fd_write, exp_str, ft_strlen(exp_str));
// 		write(fd_write, "\n", 1);
// 	}
// }

// // void	here_dic_helper(t_heredoc *here)
// // {
// // 	char	*str;

// // 	handle_signals();
// // 	while (1)
// // 	{
// // 		str = readline("> ");
// // 		if (!str)
// // 		{
// // 			close(here->fd_write);
// // 			exit(0);
// // 		}
// // 		if (ft_strcmp(here->cmd->heredoc_delimiters[here->index], str) == 0)
// // 		{
// // 			free(str);
// // 			close(here->fd_write);
// // 			exit(0);
// // 		}
// // 		write_heredoc(here->fd_write, str, here->env, here->cmd->flag);
// // 		free(str);
// // 	}
// // }

// void	write_heredoc(int fd_write, char *str, t_env **env, int flag)
// {
// 	char	*exp_str;

// 	if (flag)
// 	{
// 		write(fd_write, str, ft_strlen(str));
// 		write(fd_write, "\n", 1);
// 	}
// 	else
// 	{
// 		exp_str = expand_token(str, env);
// 		write(fd_write, exp_str, ft_strlen(exp_str));
// 		write(fd_write, "\n", 1);
// 	}
// }


// int redirect_heredoc(t_cmd *cmds)
// {
//     t_heredoc	*here;
//     char        *input;

//     here = cmds->heredocs;
//     handle_signals();
// 	while (1)
//     {
//         input = readline("> ");
//         if (!input)
//         {
//             close(here->fd_write);
//             ft_exit(0);
//         }
//         if (ft_strcmp(cmds->heredocs->delimiter, input) == 0)
//         {
//             // free(input);
//             close(here->fd_write);
//             return (0);
//         }
//         write_heredoc(here->fd_write, input, here->env, here->flag);
//         // free(input);
//     }
        
//     return (0);
// }