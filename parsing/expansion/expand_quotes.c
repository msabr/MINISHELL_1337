#include "../../minishell.h"

// Retire les quotes d'une string (simple ou double)
char *remove_quotes(const char *str, char quote_char)
{
    size_t len = ft_strlen(str);
    char *result = ft_malloc(len + 1);
    size_t j = 0;
    for (size_t i = 0; i < len; i++)
        if (str[i] != quote_char)
            result[j++] = str[i];
    result[j] = '\0';
    return result;
}

// Expansion à l'intérieur de guillemets doubles
char *expand_quoted_string(const char *str, t_env **env, int last_status)
{
    int i = 0;
    char *result = ft_strdup("");
    while (str[i])
    {
        if (str[i] == '\\' && (str[i + 1] == '$' || str[i + 1] == '\"'))
        {
            char *tmp = handle_escape_in_dquotes(str, &i);
            char *joined = strjoin_and_free(result, tmp);
            result = joined;
        }
        else if (str[i] == '$')
        {
            char *tmp = handle_dollar_expansion(str, &i, env, last_status);
            char *joined = strjoin_and_free(result, tmp);
            result = joined;
        }
        else
        {
            size_t start = i;
            while (str[i] && str[i] != '$' && str[i] != '\\')
                i++;
            char *tmp = ft_strndup(str + start, i - start);
            char *joined = strjoin_and_free(result, tmp);
            result = joined;
        }
    }
    return result;
}

// Expansion hors quotes
char *expand_unquoted_string(const char *str, t_env **env, int last_status)
{
    int i = 0;
    char *result = ft_strdup("");
    while (str[i])
    {
        if (str[i] == '\\')
        {
            char *tmp = handle_escape_unquoted(str, &i);
            char *joined = strjoin_and_free(result, tmp);
            result = joined;
        }
        else if (str[i] == '$')
        {
            char *tmp = handle_dollar_expansion(str, &i, env, last_status);
            char *joined = strjoin_and_free(result, tmp);
            result = joined;
        }
        else
        {
            size_t start = i;
            while (str[i] && str[i] != '$' && str[i] != '\\')
                i++;
            char *tmp = ft_strndup(str + start, i - start);
            char *joined = strjoin_and_free(result, tmp);
            result = joined;
        }
    }
    return result;
}