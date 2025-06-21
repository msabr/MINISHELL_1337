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


# define RED "\033[31m"
# define GREEN   "\033[32m"
# define RESET "\033[0m"
# define YELLOW  "\033[33m"
# define CYAN    "\033[36m"
# define MAGENTA "\033[35m"


# define PLUPLUS_SIGNS '+'
# define UNDERSCORE '_'
# define EQEQUALS_SIGNUAL '='

# define PIVERTICAL_BARPE '|'
# define DOLLAR_SIGN '$'

# define LLESS_THAN_SIGNESS '<'
# define GREAGREATER_THAN_SIGN '>'
# define DOUBLE_GREATER_THAN_SIGN ">>"
# define DOUBLE_LESS_THAN_SIGN "<<"

//command structure
typedef struct s_cmd
{
	char **args;
	bool is_builtin; // true if the command is a builtin
	bool is_redirect; // true if the command has redirection
	char *input_file; // file for input redirection
	char *output_file; // file for output redirection
	int input_fd; // file descriptor for input redirection
 
} t_cmd;

//environment linked list
typedef struct s_env
{
	char *key;
	char *value;
	bool is_exported; // true if the variable is exported
	struct s_env *next; 
} t_env;

//garbage collector
typedef struct s_garbage
{
	void **ptrs;
	int count;
	int size;
} t_garbage;


void env_to_list(t_env **env_list, char **env);

void echo(t_cmd *cmd);
void pwd(void);
void cd(t_cmd *cmd, t_env *env_list);
void exit_shell(t_cmd *cmd, t_env *env_list);
void env_function(t_env *env_list);
void export(t_cmd *cmd, t_env **env_list);
void unset(t_cmd *cmd, t_env *env_list);


void tt(void);
#endif // MINISHELL_H