#include "../../minishell.h"

// Expansion d'une variable d'environnement classique $VAR
char *expand_env_variable(const char *str, int *index, t_env **env)
{
    int start = ++(*index);
    while (str[*index] && (ft_isalnum(str[*index]) || str[*index] == '_'))
        (*index)++;
    char *var = strndup(str + start, *index - start);
    char *val = get_env_value(env, var);
    // free(var);
    if (!val)
        return strdup("");
    return strdup(val);
}

char *expand_braced_variable(const char *str, int *index, t_env **env, int last_status)
{
    int start = *index + 2;
    int end = start;
    (void)last_status; 
    while (str[end] && str[end] != '}')
        end++;
    char *var = strndup(str + start, end - start);
    char *val = get_env_value(env, var);
    (*index) = end + 1;
    // free(var);
    if (!val)
        return strdup("");
    return strdup(val);
}

// $?
char *expand_exit_status(int last_status)
{
    char buf[12];

    ft_putstr_fd(ft_itoa(last_status), 1);
    return strdup(buf);
}

// Expansion d'un $ dans une chaîne
char *handle_dollar_expansion(const char *str, int *index, t_env **env, int last_status)
{
    if (str[*index + 1] == '?')
    {
        (*index) += 2;
        return expand_exit_status(last_status);
    }
    else if (str[*index + 1] == '{')
    {
        return expand_braced_variable(str, index, env, last_status);
    }
    // PATCH: $"" ou $'' (expansion vide, puis USER)
    else if ((str[*index + 1] == '"' && str[*index + 2] == '"') ||
             (str[*index + 1] == '\'' && str[*index + 2] == '\''))
    {
        *index += 3;
        return strdup("");
    }
    // PATCH: $[digit] (expansion vide, saute le $ et le chiffre)
    else if (ft_isdigit(str[*index + 1]))
    {
        *index += 2;
        return strdup("");
    }
    else if (ft_isalpha(str[*index + 1]) || str[*index + 1] == '_')
    {
        return expand_env_variable(str, index, env);
    }
    else
    {
        // $ tout seul ou $ suivi de caractère non reconnu
        char *result = ft_malloc(2);
        result[0] = str[(*index)++];
        result[1] = '\0';
        return result;
    }
}