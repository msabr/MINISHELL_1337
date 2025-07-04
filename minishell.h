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

# include <signal.h>
#include <termios.h>

extern int  g_status;
// extern int  g_status;

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
typedef enum e_token_type
{
	TOKEN_WORD,         // generic word (command or argument)
	TOKEN_PIPE,         // |
	TOKEN_REDIR_IN,     // <
	TOKEN_REDIR_OUT,    // >
	TOKEN_REDIR_APPEND, // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_SQUOTE,       // '
	TOKEN_DQUOTE,       // "
	TOKEN_VARIABLE,     // $VAR
	TOKEN_EOF           // end of input
}   t_token_type;


typedef struct s_token
{
	char            *value;        // content of the token
	t_token_type    type;          // type of token
	bool            space_after;   // true if space after (useful for expansion/merging)
	struct s_token  *next;         // next token in list
	struct s_token  *prev;         // previous token (optional, for easier parsing)
}   t_token;


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

typedef struct s_redir
{
	t_token_type       type;
	char              *filename;
	int			   fd;
	int			   fd2;
	struct s_redir    *next;
} t_redir;

typedef struct s_heredoc
{
	char            *delimiter; 
	char            *content;
	struct s_heredoc *next;
} t_heredoc;

typedef struct s_cmd
{
	char            **args; 
	t_redir         *redirs; 
	t_heredoc       *heredocs;
	int             exit_status;
	struct s_cmd    *next;
}   t_cmd;

// Struct to pass execution arguments



// token type detection functions

// ---------------------------------------


// token utils
t_token *lst_new_token(const char *value, t_token_type type, bool space_after);
void lst_add_back(t_token **list, t_token *new);
bool is_operator(char c);
bool is_double_operator(const char *s);
t_token_type get_operator_type(const char *s);
void free_token_list(t_token *head);
void print_token_list(t_token *list) ;


//
t_token *lexer(const char *input);
void skip_whitespace(const char *input, size_t *i);
size_t parse_word(const char *input, size_t i, t_token **head);
size_t parse_quote(const char *input, size_t i, t_token **head);
size_t parse_operator(const char *input, size_t i, t_token **head);
size_t parse_variable(const char *input, size_t i, t_token **head) ;

t_token	*lexer2(const char *input);
size_t	parse_quote(const char *input, size_t i, t_token **head); 
int		check_syntax_errors(t_token *tokens, const char *input);
void	syntax_error(const char *msg);
void	ft_set_status(int status);
int		ft_s_ret(int set);
// char	*ft_strncpy(char *dest, const char *src, size_t n);

t_cmd *parse_tokens_to_cmds(t_token *tokens);
void print_cmds(t_cmd *cmds);

// ---------------------------------------

//built-in functions
bool	is_builtin(char *cmd);
void	execve_builtin(char **args, t_env **env_list);
bool	is_valid_key(char *key);
void	cd(t_cmd *cmd, t_env **env_list);
void	echo(t_cmd *cmd);
void	env_function(t_env *env_list);
void	exit_shell(t_cmd *cmd);
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
t_env	*env_to_list(char **env);
t_env	*search_env_node(t_env *list_head, const char *search_key);
void	configure_environment(t_env **env_list, char **env_array);
char	*get_env_value(t_env **env_list, const char *key);
void	add_env_value(t_env **env_list, char *key, char *value);
void	add_temporary_env_value(t_env **env_list, char *key);
char **list_to_env(t_env *env_list);

//redirection functions
bool	ft_is_dir(const char *path);
int exec_multiple_pipes(t_cmd *cmds, t_env **env_list);
int execve_simple_cmd(t_cmd *cmds, t_env **env_list);
char *get_path(char *cmd, t_env *env_list);
void	tt(void);

//signals functions
int 	handle_exit_status(pid_t pid);
void	sig_ctl_c(int sig);
void	ft_handler_signal(void);

bool is_redirection(t_cmd *cmds);
void redirect_stdin(t_cmd *cmd);
void redirect_overwrite(t_cmd *cmd);
void redirect_append(t_cmd *cmd);

bool handle_redirections(t_cmd *cmds);

#endif // MINISHELL_H