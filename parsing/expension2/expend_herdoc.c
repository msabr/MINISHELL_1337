# include "../../minishell.h"


char *expand_heredoc_content(const char *str, t_env **env, int last_status, const char *delimiter)
{
    if (strchr(delimiter, '\'') || strchr(delimiter, '"'))  // PATCH: quoted delimiter
        return ft_strdup(str);
    (void) last_status;
    return expand_variables_in_word((char *)str, *env);
}