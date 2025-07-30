/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 06:22:31 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 10:37:32 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			space_after;
	int				quoted;
	int				expended;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_data
{
	t_token	*token;
	t_token	*last_token;
	t_env	*env;
	int		i;
	int		error;
	int		exit_status;
}	t_data;

typedef struct s_expand
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*result;
}	t_expand;

typedef struct s_t_tk_args
{
	const char		*val;
	t_token_type	type;
	bool			space;
	int				expneded;
}	t_tk_args;

/* ..............: Syntaxe error :....................*/
int				is_valid_word(char *value);
const char		*token_repr(t_token *tok);
int				check_unclosed_quote(const char *input);
void			error_syntax(const char *token);
int				check_syntax_errors(t_token *tokens, const char *input);

/* ..............: parse_cmd :....................*/

void			lst_remove_token(t_token **head, t_token *node);
void			lst_insert_before(t_token **head, t_token *node,
					t_token *new_token);
int				is_assignment_word(const char *str);
t_token			*split_and_insert(t_token **tokens, t_token *current);
void			field_split_tokens(t_token **tokens);

int				is_arg_token(t_token *tok);
int				is_redir(t_token_type t);
size_t			arg_total_len(t_token *tok);
int				count_args(char **args);
int				add_argument(char ***args, char *new_arg);

t_cmd			*new_command(void);
int				handle_redir(t_cmd *cmd, t_token **tok);
int				is_only_spaces(const char *str);

t_redir			*new_redir(t_token_type type, char *filename, int quoted_name);
int				add_redirection(t_redir **redir, t_token_type type, char *fname,
					int q_name);

int				add_command(t_cmd **cmds, t_cmd *new);
char			*merge_argument(t_token **ptok);

int				process_split_args(t_cmd *current, char *arg);
int				process_export_logic(t_cmd *current, char *arg, int was_quoted);
int				process_ar_tk_helper(t_token **ptok, t_cmd *curr, char *arg,
					t_token *tok);
int				process_argument_token(t_token **ptok, t_cmd *current);
int				parse_tokens_iteration(t_token **ptok, t_cmd **current,
					t_cmd **cmds);
int				parse_tokens_loop(t_token *tok, t_cmd **cmds);
t_cmd			*parse_tokens_to_cmd2s(t_token *tokens);

void			remove_empty_word_tokens(t_token **tokens);

t_tk_args		new_tk_args(const char *val, t_token_type type, bool spc,
					int expd);
void			handle_quote(const char *input, size_t *i, t_token **head);
size_t			variable_length(const char *input, size_t i);

int				is_whitespace(char c);
char			*ft_strndup(const char *src, size_t n);
bool			is_operator(char c);
bool			is_double_operator(const char *s);
t_token_type	get_operator_type(const char *s);

int				is_quote_closed(const char *input, size_t i, char quote);
char			*extract_word_value(const char *input, size_t *i, size_t *len);
void			add_word_token(const char *input, size_t *i, t_token **head);
void			add_token(t_token **head, t_tk_args *args);
void			add_token_quoted(t_token **head, t_tk_args *args);

char			*extract_quote_value(const char *inpt, size_t *i, char qte);
void			handle_variable(const char *input, size_t *i, t_token **head);
void			handle_operator(const char *input, size_t *i, t_token **head);
void			handle_quote(const char *input, size_t *i, t_token **head);
t_token			*lexer(const char *input);

t_token			*lst_new_token(const char *val, t_token_type type, bool space,
					int expd);
void			lst_add_back(t_token **list, t_token *new);
void			remove_hel(t_token **tok, t_token **prev, t_token *curr,
					t_token *next);
void			remove_empty_word_tokens(t_token **tokens);

void			append_exit_code(t_expand *xp);
void			append_other_dollar(t_expand *xp, char *str);
void			handle_dollar(t_expand *xp, char *str, t_env *env);
char			*expand_variables_in_word(char *str, t_env *env);
void			merge_collapsed_tokens(t_token *tokens);

void			append_env_var2(t_expand *xp, char *str, t_env *env);
void			append_other_dollar(t_expand *xp, char *str);
void			handle_dollar(t_expand *xp, char *str, t_env *env);
char			*expand_variables_in_string(const char *str, t_env *env);

int				count_dollars(const char *str);
char			*expand_many_dollars(const char *str, t_env *env);
char			*extract_env_key(char *str, t_expand *xp, size_t *varlen);
void			append_env_var(t_expand *xp, char *str, t_env *env);
size_t			calc_var_env_len(const char *str, size_t *i, t_env *env);

size_t			get_varlen(char *str, size_t i);
char			*get_env_key(char *str, size_t i, size_t varlen);
void			fix_token_types_after_expansion(t_token *tokens);
void			append_env_value2(t_expand *xp, char *val);
char			*get_expanded_variable(t_token *curr, t_env *env);

int				was_originally_quoted(t_token *token);
void			expansion_handle_heredoc(t_token *curr);
int				is_in_heredoc(t_token *token);

void			expand_var_token(t_token *token, t_env *env);
int				find_question(const char *str);
int				find_dollar(const char *str);
int				find_number(const char *str);
char			*get_variable(const char *str, int index);

char			*get_value(t_env *env, const char *key_val);
char			*replace_question(char *str, int index);
char			*replace_number(char *str, int index);
char			*replace_dollar(char *str, int index, t_env *env);

void			expand_dquote_token(t_token *token, t_env *env);
char			*remove_squotes(char *str);
char			*remove_dquotes(char *str);
void			merge_variable_tokens(t_token *tokens);
void			expand_squote_token(t_token *token);

void			number_before_dollar(t_token *token);
void			convert_exit_code(t_token *token);
char			*get_env_dup_value(t_env *env, char *key_val);
void			expand_env_dollar(t_token *token, t_env *env);
void			fix_dollar_doublequote_tokens(t_token **head);

char			*extract_env_key2(const char *str, size_t i, size_t varlen);
size_t			calc_exit_code_len(size_t *i);
size_t			calc_other_dollar_len(const char *str, size_t *i);
size_t			calc_expanded_len(const char *str, t_env *env);
void			append_exit_code(t_expand *xp);

char			*expand_helper(const char *v_name, int d_cnt, t_env *env,
					t_token *curr);
void			expansion_all_tokens(t_token *tokens, t_env *env);
void			handle_quote(const char *input, size_t *i, t_token **head);

t_token			*lexer(const char *input);
void			expansion_all_tokens(t_token *tokens, t_env *env);
void			field_split_tokens(t_token **tokens);
void			merge_collapsed_tokens(t_token *tokens);
int				check_syntax_errors(t_token *tokens, const char *input);
t_cmd			*parse_tokens_to_cmd2s(t_token *tokens);

#endif