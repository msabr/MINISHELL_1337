#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "Libft/libft.h"

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

typedef struct s_env
{
	char            *key;
	char            *value; // Environment variable value
	struct s_env    *next;  // Pointer to the next environment variable
} t_env;

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
	char **args;          // Command arguments
	char *input_file;    // Input redirection file
	char *output_file;   // Output redirection file
	int append;          // Append mode for output redirection
} t_cmd;
// Lexer helper functions

// garbage collector
typedef struct s_gc
{
	void			*ptr;
	struct s_gc		*next;

}	t_gc;


void env_to_list(t_env **env_list, char **env);

void echo(t_cmd *cmd);
void pwd(void);
void cd(t_cmd *cmd, t_env *env_list);
void exit_shell(t_cmd *cmd, t_env *env_list);
void env_function(t_env *env_list);
void export(t_cmd *cmd, t_env *env_list);
void unset(t_cmd *cmd, t_env *env_list);

// parsing functions

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



#endif // MINISHELL_H
