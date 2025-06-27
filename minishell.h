/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:14:02 by msabr             #+#    #+#             */
/*   Updated: 2025/06/27 21:11:42 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"

# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

#include <termios.h>



# define RESET		"\033[0m"
# define RED		"\033[31m"
# define GREEN  	"\033[32m"
# define YELLOW 	"\033[33m"
# define CYAN		"\033[36m"
# define MAGENTA	"\033[35m"

# define PLUPLUS_SIGNS '+'
# define UNDERSCORE '_'
# define EQEQUALS_SIGNUAL '='

# define PIVERTICAL_BARPE '|'
# define DOLLAR_SIGN '$'

# define LLESS_THAN_SIGNESS '<'//lless_than_sign
# define GREAGREATER_THAN_SIGN '>'  //greagreater_than_sign
# define DOUBLE_GREATER_THAN_SIGN ">>" //double_greater_than_sign
# define DOUBLE_LESS_THAN_SIGN "<<" //double_less_than_sign

//command structure
typedef struct s_cmd
{
	char	**args;
	bool	is_builtin;
	bool	is_redirect;
	bool	is_pipe;
	char	*input_file;
	char	*output_file;
	int		input_fd;
	int		output_fd;
	struct s_cmd *next;
}	t_cmd;

//environment linked list
typedef struct s_env
{
	char			*key;
	char			*value;
	bool			export_variable;
	struct s_env	*next;
}	t_env;

//garbage collector
typedef struct s_garbage
{
	void	**ptrs;
	int		count;
	int		size;
}	t_garbage;

//built-in functions
bool	is_bultins(char *cmd);
void	execve_builtin(char **args, t_env **env_list);
bool	is_valid_key(char *key);
void	cd(t_cmd *cmd, t_env **env_list);
void	echo(t_cmd *cmd);
void	env_function(t_env *env_list);
void	exit_shell(t_cmd *cmd, t_env *env_list);
void	sort_env_list(t_env **env_list);
void	append_env_value(t_env **env_list, char *key, char *value);
char	*set_key(const char *arg);
void	export_withot_args(t_env **env_list);
void	export(t_cmd *cmd, t_env **env_list);
char	*get_pwd(void);
char	*get_pwd_from_env(t_env **env_list);
void	pwd(t_env **env_list);
void	unset(t_cmd *cmd, t_env **env_list);

//environment functions
void	env_to_list(t_env **env_list, char **env);
t_env	*search_env_node(t_env *list_head, const char *search_key);
void	configure_environment(t_env **env_list, char **env_array);
char	*get_env_value(t_env **env_list, const char *key);
void	add_env_value(t_env **env_list, char *key, char *value);
void	add_temporary_env_value(t_env **env_list, char *key);

//redirection functions
bool	is_redirection(char *cmd);
void	handle_redirection(t_cmd *cmd);
void	handle_input_redirection(t_cmd *cmd);
void	handle_output_redirection(t_cmd *cmd);

void execve_cmd(char **args, t_env **env_list, char **env);
char *get_path(char *cmd, t_env *env_list);
void	tt(void);

#endif // MINISHELL_H