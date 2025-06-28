/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 14:14:02 by msabr             #+#    #+#             */
/*   Updated: 2025/06/28 22:31:45 by msabr            ###   ########.fr       */
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
# define DOUBLE_QUOTES '"'
# define SINGLE_QUOTES '\''
# define BACKSLASH '\\'
# define SPACE ' '
# define TAB '\t'
# define NEWLINE '\n'
# define SEMICOLON ';'
# define AMPERSAND '&'
# define QUESTION_MARK '?'
# define EXCLAMATION_MARK '!'


# define LLESS_THAN_SIGNESS '<'//lless_than_sign
# define GREAGREATER_THAN_SIGN '>'  //greagreater_than_sign
# define DOUBLE_GREATER_THAN_SIGN ">>" //double_greater_than_sign
# define DOUBLE_LESS_THAN_SIGN "<<" //double_less_than_sign

// parser structures
typedef enum e_type
{
	SQUOTE,
	SPCE,
	DQUOTE,
	VAR,
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND,
} t_type; // Correctly ends the enum declaration

typedef struct s_token
{
	char            *value;
	t_type         type;
	int             a_sp;
	struct s_token  *next;
} t_token;

//environment linked list
typedef struct s_env
{
	char			*key;
	char			*value;
	bool			export_variable;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	t_token        *token;
	t_token        *last_token;
	t_env          *env;
	int            i;
	int            error;
	int            exit_status;
} t_data;

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

// Lexer helper functions

// garbage collector
typedef struct s_gc
{
	void			*ptr;
	struct s_gc		*next;

}	t_gc;


//garbage collector
typedef struct s_garbage
{
	void	**ptrs;
	int		count;
	int		size;
}	t_garbage;



// token type detection functions
int     is_shell_operator(const char *str);
int     is_shell_variable(const char *str);
int     has_single_quotes(const char *str);
int     has_double_quotes(const char *str);
int     is_only_whitespace(const char *str);
t_type  classify_token_content(const char *value);
t_type  get_operator_category(const char *op);

// token list functions
t_token *ft_lstnew_token(char *value, t_type type, int a_sp);
void    ft_lstadd_back_token(t_token **lst, t_token *new);
t_token *ft_last_token(t_token **lst);
void    ft_free_tokens(t_token *tokens);


int     skip_whitespace_chars(const char *input, int pos);
int     process_next_element(const char *input, int pos, t_token **token_list);
int     detect_operator_length(const char *input, int start_pos);
int     handle_operator_token(const char *input, int start_pos, int op_length, t_token **token_list);
int     handle_word_token(const char *input, int start_pos, t_token **token_list);
char    *extract_operator_string(const char *input, int pos, int op_length);
int     has_space_after(const char *input, int pos);
t_token *create_token_from_string(char *value, int space_after);
int     find_word_end(const char *input, int start_pos);
char    *extract_word_string(const char *input, int start_pos, int end_pos);






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