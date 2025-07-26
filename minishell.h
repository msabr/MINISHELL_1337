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

# include <termios.h>
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
	int		heredoc_num;
	int		fd_write;
	int		index;
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

/**..................................................
t_token	*lexer(const char *input);
// Lexing main API
t_token	*lexer(const char *input);

// Helpers & handlers
int			is_whitespace(char c);
bool		is_operator(char c);
t_token_type	get_operator_type(const char *s);
char		*ft_strndup(const char *src, size_t n);

void		add_token(t_token **head, const char *val, t_token_type type, bool space);
void		add_token_quoted(t_token **head, const char *val, t_token_type type, bool space, int quoted);
t_token		*lst_new_token(const char *value, t_token_type type, bool space_after);
void		lst_add_back(t_token **list, t_token *new);

void		error_syntax(const char *token);

//..................................................*/
int		check_syntax_errors(t_token *tokens, const char *input);
void	error_syntax(const char *token);
int     count_args(char **args);
t_token *find_token_for_value(t_token *tokens, char *value);
char    *strjoin_and_free(char *s1, char *s2);
// Parsing principal
t_cmd	*parse_tokens_to_cmd2s(t_token *tokens);
// t_cmd	*parse_tokens_to_cmd2s(t_token *tokens, t_env *env);
void	parse_export_handler(t_token *tokens, t_cmd *cmd, t_env *env);

// Supprime les tokens vides de la liste chainée
void	remove_empty_token(t_token **tokens);

// new expending

void	expansion_all_tokens(t_token *tokens, t_env *env);
int		was_originally_quoted(t_token *token);
void	expand_var_token(t_token *token, t_env *env);
void	expand_dquote_token(t_token *token, t_env *env);
void	expand_squote_token(t_token *token);
int		is_in_heredoc(t_token *token);
void	convert_exit_code(t_token *token);
void	number_before_dollar(t_token *token);
void	expand_env_dollar(t_token *token, t_env *env);
char	*remove_dquotes(char *str);
char	*remove_squotes(char *str);
void	fix_dollar_doublequote_tokens(t_token **head);
char	*expand_many_dollars(const char *str, t_env *env);
void	merge_variable_tokens(t_token *tokens);
char	*expand_variables_in_word(char *str, t_env *env);
void	merge_collapsed_tokens(t_token *tokens);
char	*expand_token(const char *str, t_env *env);
char	*expand_env_var(const char *var, t_env *env);

// Expansion du code de retour ($?)
char	*expand_exit_code(void);
// Découpe une string à chaque '$' (pour gérer les collages)
char	**split_by_dollar(const char *str);
// Expansion d'un morceau de string (ex: "$USER", "$?", "abc")
char	*expand_piece(const char *piece, t_env *env);

// Helpers internes (utilisables pour tests ou extension)
int		is_arg_token(t_token *tok);
int		is_redir(t_token_type t);
char	*merge_argument(t_token **ptok);
int		add_argument(char ***args, char *new_arg);
int		add_redirection(t_redir **redir, t_token_type type, char *filename,int quoted_name);
int		add_command(t_cmd **cmds, t_cmd *new);
t_cmd	*new_command(void);
void	free_cmd_list(t_cmd *cmds); // Gestion mémoire
void	remove_empty_token_head(t_token **tokens);
t_redir *new_redir(t_token_type type, char *filename,int quoted_name);
//..................................................*/
// token utils
t_token*		lst_new_token(const char *value, t_token_type type, bool space_after,int expended);
void			lst_add_back(t_token **list, t_token *new);
bool			is_operator(char c);
bool			is_double_operator(const char *s);
void			free_token_list(t_token *head);
void			print_token_list(t_token *list) ;
t_token_type	get_operator_type(const char *s);
int				is_whitespace(char c);
void			add_token(t_token **head, const char *val, t_token_type type, bool space,int expneded);
void			add_token_quoted(t_token **head, const char *val, t_token_type type, bool space, int expneded);
void			error_syntax(const char *token);
//
t_token*		lexer(const char *input);
void			skip_whitespace(const char *input, size_t *i);
size_t			parse_word(const char *input, size_t i, t_token **head);
size_t			parse_quote(const char *input, size_t i, t_token **head);
size_t			parse_operator(const char *input, size_t i, t_token **head);
size_t			parse_variable(const char *input, size_t i, t_token **head) ;

t_token			*lexer2(const char *input);
size_t			parse_quote(const char *input, size_t i, t_token **head); 
int				check_syntax_errors(t_token *tokens, const char *input);
void			syntax_error(const char *msg);
int				ft_s_ret(int set);

// char	*ft_strncpy(char *dest, const char *src, size_t n);

// t_cmd *parse_tokens_to_cmds(t_token *tokens);
void	print_cmds(t_cmd *cmds);
void	expand_token_list_v2(t_token *tokens, t_env **env, int last_status);
bool	needs_expansion(t_token *token);
void	expand_single_token(t_token *token, t_env **env, int last_status);

// Gestion des quotes
char	*remove_quotes(const char *str, char quote_char);
char	*expand_quoted_string(const char *str, t_env **env, int last_status);
char	*expand_unquoted_string(const char *str, t_env **env, int last_status);

// Gestion des échappements
char	*handle_escape_in_dquotes(const char *str, int *index);
char	*handle_escape_unquoted(const char *str, int *index);

// Expansion des variables
char	*handle_dollar_expansion(const char *str, int *index, t_env **env, int last_status);
char	*expand_env_variable(const char *str, int *index, t_env **env);
char	*expand_braced_variable(const char *str, int *index, t_env **env, int last_status);

// Fonctions utilitaires
bool	has_special_chars(const char *str);
char	**split_words(const char *str);
int		count_words(const char *str);
char	*clean_expansion_result(char *str);
char	*strjoin_and_free(char *s1, char *s2);
void	free_words(char **words);
char	*expand_dollars(const char *input, t_env **env);
char	*expand_heredoc_content(const char *str, t_env **env, int last_status, const char *delimiter);
int		count_dollars(const char *str, int index);
char	*reduce_odd_dollars_and_handle_edge(const char *str);
// ---------------------------------------

//built-in functions
bool	is_builtin(char *cmd);
int		execve_builtin(t_cmd *cmd, t_env **env_list);
int		cd(t_cmd *cmd, t_env **env_list);
void	echo(t_cmd *cmd);
void	env_function(t_env *env_list);
int		exit_shell(t_cmd *cmd);
char	*set_key(const char *arg);
int		export(t_cmd *cmd, t_env **env_list);
// void	export(char **args, t_env **env_list);
void	pwd(t_env **env_list);
int		unset(t_cmd *cmd, t_env **env_list);
char	*ft_getcwd(void);

//environment functions
t_env	*env_to_list(char **env);
char	**list_to_env(t_env *env_list);
t_env	*find_env_node(char *search_key, t_env *list_head);
char	*get_env_value(t_env **env_list, const char *key);
void	add_env_value(t_env **env_list, char *key, char *value);
void	add_temporary_env_value(t_env **env_list, char *key);
void	remove_env_variable(t_env **env_list, const char *key);
void	configure_environment(t_env **env_list, char **env_array);
int		size_of_env_list(t_env *env_list);
void	free_env_list(t_env *env_list);
t_env	*copy_env(t_env *env_list);

// execution functions
bool	 is_directory(const char *path);
int		exec_multiple_pipes(t_cmd *cmds, t_env **env_list);
int		get_exec_path(t_cmd *cmds, t_env **env_list, char **path);
int		execve_simple_cmd(t_cmd *cmds, t_env **env_list);
char	*get_path(char *cmd, t_env *env_list);

//signals functions
void	tt(void);
int 	handle_exit_status(pid_t pid, int status);
void	handel_ctl_c(int sig);
void	ft_handler_signal(void);
void	set_default_signals(void);

//exit status functions
int		*ft_get_status(void);
void	ft_set_status(int status);
void	ft_exit(int status);
void	ft_set_and_exit(int status);

//redirection functions
bool	is_redirection(t_cmd *cmds);
int		redirect_heredoc(t_cmd *cmd, t_env *env_list);
void	save_std_fds(t_cmd *cmds);
void	restore_std_fds(t_cmd *cmds);
int		redirect_stdin(char *file_name);
int		redirect_overwrite(char *file_name);
int		redirect_append(char *file_name);
bool	handle_redirections(t_cmd *cmds, t_env *env);
int	preprocess_heredocs(t_cmd *cmds, t_env **env);
//error handling functions
void	ft_perror(char *errn);
int		print_dir_error(char *cmd);
int		print_cmd_not_found_error(char *cmd);
void	print_exit_error(const char *arg);
//main functions
void	main_loop(t_env **env_list);

#endif
