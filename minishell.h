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
typedef struct s_token
{
	t_type			type;
	char			*value;
	int				quoted;
	// hiden attribute like sofian say 
	struct s_token	*next;
	struct s_token	*prev;
}			t_token;

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
}	t_type;

typedef struct s_data
{
	t_token		*token;
	t_token		*last_token;
	t_env		*env;
	int			i;
	int			error;
	int			exit_status;
}			t_data;

// 

typedef struct s_cmd
{
	char **args;          // Command arguments
	char *input_file;    // Input redirection file
	char *output_file;   // Output redirection file
	int append;          // Append mode for output redirection
} t_cmd;

typedef struct s_env
{
	char *key;
	char *value;         // Environment variable value
	struct s_env *next; // Pointer to the next environment variable
} t_env;

void env_to_list(t_env **env_list, char **env);

void echo(t_cmd *cmd);
void pwd(void);
void cd(t_cmd *cmd, t_env *env_list);
void exit_shell(t_cmd *cmd, t_env *env_list);
void env_function(t_env *env_list);
void export(t_cmd *cmd, t_env *env_list);
void unset(t_cmd *cmd, t_env *env_list);


// parsing functions
int check_closedqt(const char *str)
#endif // MINISHELL_H