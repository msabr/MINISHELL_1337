#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

#endif // MINISHELL_H