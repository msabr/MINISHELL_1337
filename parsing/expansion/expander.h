#ifndef EXPANDER_H
#define EXPANDER_H

#include "../../minishell.h"

// Fonctions principales
void expand_token_list_v2(t_token *tokens, t_env **env, int last_status);
bool needs_expansion(t_token *token);
void expand_single_token(t_token *token, t_env **env, int last_status);

// Gestion des quotes
char *remove_quotes(const char *str, char quote_char);
char *expand_quoted_string(const char *str, t_env **env, int last_status);
char *expand_unquoted_string(const char *str, t_env **env, int last_status);

// Gestion des échappements
char *handle_escape_in_dquotes(const char *str, int *index);
char *handle_escape_unquoted(const char *str, int *index);

// Expansion des variables
char *handle_dollar_expansion(const char *str, int *index, t_env **env, int last_status);
char *expand_env_variable(const char *str, int *index, t_env **env);
char *expand_braced_variable(const char *str, int *index, t_env **env, int last_status);

// Fonctions utilitaires
bool has_special_chars(const char *str);
char **split_words(const char *str);
int count_words(const char *str);
char *clean_expansion_result(char *str);

#endif