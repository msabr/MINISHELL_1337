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

/*................: parse_export.c:......................*/

void	    lst_remove_token(t_token **head, t_token *node);
void	    lst_insert_before(t_token **head, t_token *node, t_token *new_token);
int         is_assignment_word(const char *str);
t_token	    *split_and_insert(t_token **tokens, t_token *current);
void	    field_split_tokens(t_token **tokens);

/*................: parse_utils.c:......................*/

int         is_arg_token(t_token *tok);
int         is_redir(t_token_type t);
size_t	arg_total_len(t_token *tok);
int         count_args(char **args);
int         add_argument(char ***args, char *new_arg);
t_cmd	    *new_command(void);
/*................: parse_utils2.c:......................*/
static int	handle_redir(t_cmd *cmd, t_token **tok);
static int	is_only_spaces(const char *str);
static int	process_split_args(t_cmd *current, char *arg);
static void	remove_empty_word_tokens(t_token **tokens);

/*................: parse_helper.c:......................*/

t_redir     *new_redir(t_token_type type, char *filename, int quoted_name);
int         add_redirection(t_redir **redir, t_token_type type, char *fname, int q_name);
int         add_command(t_cmd **cmds, t_cmd *new);
char	    *merge_argument(t_token **ptok);
/*................: parser.c:......................*/
int	parse_tokens_iteration(t_token **ptok, t_cmd **current, t_cmd **cmds);
int	process_ar_tk_helper(t_token **ptok, t_cmd *curr, char *arg, t_token *tok);
#endif