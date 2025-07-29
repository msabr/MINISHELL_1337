# include "../../minishell.h"


char *expand_heredoc_content(const char *str, t_env **env, int last_status, const char *delimiter)
{
    t_token *token;
    char *expended;
     (void) last_status;
    if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
        return ft_strdup(str);
    token = lst_new_token(str, TOKEN_WORD, false, 0);
    expansion_all_tokens(token, *env);
    expended = ft_strdup(token->value);
    return expended;
}