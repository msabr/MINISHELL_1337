#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"

# include <stdio.h>

// # include "parsing/expension2/expansion.h"

extern int	g_status;



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
	int				quoted;
	int				expended;
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

typedef struct s_heredoc
{
	char	*delimiter;
	int		fd_read;
	int		fd_write;
	char	*tmp_file;
	int		flag; // 1 if the heredoc is quoted, 0 if not
	t_env	**env;
}	t_heredoc;

typedef struct s_redir
{
	t_token_type	type;
	t_heredoc		*heredoc;
	char			*filename;
	int				fd_in;
	int				fd_out;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	bool			in_pipe; 
	struct s_cmd	*next;
}   t_cmd;
char			*expand_heredoc_content(const char *str, t_env **env,
					const char *del);
t_token	*lexer(const char *input);
void	expansion_all_tokens(t_token *tokens, t_env *env);
void	field_split_tokens(t_token **tokens);
void	merge_collapsed_tokens(t_token *tokens);
int	check_syntax_errors(t_token *tokens, const char *input);
t_cmd	*parse_tokens_to_cmd2s(t_token *tokens);
t_cmd	*parse_input(char *input, t_env *env_list, int *status);

//main functions
void	main_loop(t_env **env_list);
void	configure_environment(t_env **env_list, char **env_array);

char	*get_env_value(t_env **env_list, const char *key);

int		*ft_get_status(void);
void	ft_set_status(int status);
#endif
