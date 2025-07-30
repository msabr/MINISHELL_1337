/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:51:10 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 12:51:36 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../minishell.h"

# include <errno.h>
# include <signal.h>
# include <termios.h>
# include <readline/history.h>
# include <readline/readline.h>

//built-in functions
bool	is_builtin(char *cmd);
int		execve_builtin(t_cmd *cmd, t_env **env_list);
char	*ft_getcwd(void);
char	*set_key(const char *arg);

//environment functions
t_env	*env_to_list(char **env);
char	**list_to_env(t_env *env_list);
t_env	*find_env_node(char *search_key, t_env *list_head);
char	*get_env_value(t_env **env_list, const char *key);
void	add_env_value(t_env **env_list, char *key, char *value);
void	add_temporary_env_value(t_env **env_list, char *key);
void	remove_env_variable(t_env **env_list, const char *key);
int		size_of_env_list(t_env *env_list);
t_env	*copy_env(t_env *env_list);

// execution functions
bool	is_directory(const char *path);
int		exec_multiple_pipes(t_cmd *cmds, t_env **env_list);
int		get_exec_path(t_cmd *cmds, t_env **env_list, char **path);
int		execve_simple_cmd(t_cmd *cmds, t_env **env_list);
char	*get_path(char *cmd, t_env *env_list);

//signals functions
int		handle_exit_status(pid_t pid, int status);
void	handel_ctl_c(int sig);
void	ft_handler_signal(void);
void	set_default_signals(void);

//exit status functions
void	ft_exit(int status);
void	ft_set_and_exit(int status);

//redirection functions
bool	is_heredoc(t_cmd *cmds);
bool	handel_heredoc(t_cmd *cmds, t_env *env);
void	close_heredoc(t_cmd *cmds);
bool	is_redirection(t_cmd *cmds);
bool	handle_redirections(t_cmd *cmds);

//file descriptor functions
void	save_std_fds(t_cmd *cmds);
void	restore_std_fds(t_cmd *cmds);

//error handling functions
void	ft_perror(char *errn);
int		print_dir_error(char *cmd);
int		print_cmd_not_found_error(char *cmd);

#endif
