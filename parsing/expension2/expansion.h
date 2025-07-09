#ifndef EXPANSION_H
# define EXPANSION_H

# include "../../minishell.h"  // <-- Ajoute ceci en tout premier !

void    expansion_all_tokens(t_token *tokens, t_env *env);
void    expand_var_token(t_token *token, t_env *env);
void    expand_dquote_token(t_token *token, t_env *env);
void    expand_squote_token(t_token *token);
int     is_in_heredoc(t_token *token);
void    convert_exit_code(t_token *token);
void    number_before_dollar(t_token *token);
void    expand_env_dollar(t_token *token, t_env *env);
char    *remove_dquotes(char *str);
char    *remove_squotes(char *str);

#endif