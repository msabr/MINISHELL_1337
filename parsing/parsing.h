#ifndef PARSING_H
#define PARSING_H

#include "../minishell.h"

/* ..............: Syntaxe error :....................*/
int             is_valid_word(char *value);
const char      *token_repr(t_token *tok);
int             check_unclosed_quote(const char *input);
void            error_syntax(const char *token);
int	            check_syntax_errors(t_token *tokens, const char *input);

/* ..............: parse_cmd :....................*/

void	    lst_remove_token(t_token **head, t_token *node);
void	    lst_insert_before(t_token **head, t_token *node, t_token *new_token);
int         is_assignment_word(const char *str);
t_token	    *split_and_insert(t_token **tokens, t_token *current);
void	    field_split_tokens(t_token **tokens);


int		is_arg_token(t_token *tok);
int		is_redir(t_token_type t);
size_t	arg_total_len(t_token *tok);
int		count_args(char **args);
int		add_argument(char ***args, char *new_arg);


t_cmd	*new_command(void);
int		handle_redir(t_cmd *cmd, t_token **tok);
int		is_only_spaces(const char *str);

t_redir	*new_redir(t_token_type type, char *filename, int quoted_name);
int		add_redirection(t_redir **redir, t_token_type type, char *fname, int q_name);
int		add_command(t_cmd **cmds, t_cmd *new);
char	*merge_argument(t_token **ptok);

int		process_split_args(t_cmd *current, char *arg);
int		process_export_logic(t_cmd *current, char *arg, int was_quoted);
int		process_ar_tk_helper(t_token **ptok, t_cmd *curr, char *arg, t_token *tok);
int		process_argument_token(t_token **ptok, t_cmd *current);
int		parse_tokens_iteration(t_token **ptok, t_cmd **current, t_cmd **cmds);
int		parse_tokens_loop(t_token *tok, t_cmd **cmds);
t_cmd	*parse_tokens_to_cmd2s(t_token *tokens);

void	remove_empty_word_tokens(t_token **tokens);

int			is_whitespace(char c);
char		*ft_strndup(const char *src, size_t n);
bool		is_operator(char c);
bool		is_double_operator(const char *s);
t_token_type	get_operator_type(const char *s);

t_token	*lexer(const char *input);
void	handle_quote_token(const char *inpt, size_t *i, char qte, t_token **hd);


int	is_quote_closed(const char *input, size_t i, char quote);
size_t	variable_length(const char *input, size_t i);
void	handle_word(const char *input, size_t *i, t_token **head);

t_token	*lst_new_token(const char *value, t_token_type type, bool space_after, int expended);
void	lst_add_back(t_token **list, t_token *new);
void	add_token(t_token **head, const char *val, t_token_type type, bool space, int expneded);
void	add_token_quoted(t_token **head, const char *val, t_token_type type, bool space, int expneded);
void	remove_empty_word_tokens(t_token **tokens);
void	remove_hel(t_token **tok, t_token **prev, t_token *curr, t_token *next);




void	expand_var_token(t_token *token, t_env *env);
int		find_question(const char *str);
int		find_number(const char *str);
int     find_question(const char *str);
char	*get_variable(const char *str, int index);
char	*get_value(t_env *env, const char *key_val);
char	*replace_question(char *str, int index);
char	*replace_number(char *str, int index);
char	*replace_dollar(char *str, int index, t_env *env);
void	expand_dquote_token(t_token *token, t_env *env);
char	*remove_squotes(char *str);
void	expand_squote_token(t_token *token);
void	convert_exit_code(t_token *token);
void	number_before_dollar(t_token *token);
void	expand_env_dollar(t_token *token, t_env *env);
char	*remove_dquotes(char *str);
void	fix_dollar_doublequote_tokens(t_token **head);
char	*expand_many_dollars(const char *str, t_env *env);
void	merge_variable_tokens(t_token *tokens);
char	*expand_variables_in_word(char *str, t_env *env);
void	merge_collapsed_tokens(t_token *tokens);

#endif