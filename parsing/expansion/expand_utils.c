#include "../../minishell.h"

// Vérifie si une chaîne a des caractères spéciaux (utile pour split)
bool has_special_chars(const char *str)
{
    if (!str)
        return false;
    while (*str)
    {
        if (*str == '$' || *str == '\'' || *str == '\"')
            return true;
        str++;
    }
    return false;
}

// Découpe une chaîne en mots
char **split_words(const char *str)
{
    return ft_split(str, ' ');
}

// int count_words(const char *str)
// {
//     char **split = split_words(str);
//     int count = 0;
//     while (split && split[count])
//         count++;
//     if (split)
//         free_words(split);
//     return count;
// }

// Nettoie le résultat d'expansion (retire quotes)
char *clean_expansion_result(char *str)
{
    if (!str)
        return NULL;
    char *cleaned = remove_quotes(str, '\'');
    char *final = remove_quotes(cleaned, '\"');
    // free(cleaned);
    return final;
}


// Helper pour concaténer et free
char *strjoin_and_free(char *a, char *b)
{
    char *joined = ft_strjoin(a, b);
    // free(a);
    // free(b);
    return joined;
}

// void free_words(char **words)
// {
//     int i = 0;
//     while (words[i])
//         free(words[i++]);
//     free(words);
// }

// Cherche la valeur d'une variable d'environnement
// char *get_env_value(t_env **env, const char *key)
// {
//     t_env *tmp = *env;
//     while (tmp)
//     {
//         if (strcmp(tmp->key, key) == 0)
//             return tmp->value;
//         tmp = tmp->next;
//     }
//     return NULL;
// }
char *expand_dollars(const char *input, t_env **env)
{
    int i = 0;
    char *result = ft_strdup("");
    while (input[i])
    {
        if (input[i] == '$')
        {
            int dollar_count = 0;
            // Compte les dollars consécutifs
            while (input[i + dollar_count] == '$')
                dollar_count++;
            // Vérifie si une variable suit la séquence de dollars
            if ((dollar_count % 2) == 1 && (ft_isalpha(input[i + dollar_count]) || input[i + dollar_count] == '_'))
            {
                // Expansion variable après un nombre impair de dollars
                int var_len = 0;
                while (ft_isalnum(input[i + dollar_count + var_len]) || input[i + dollar_count + var_len] == '_')
                    var_len++;
                char *var_name = ft_substr(input, i + dollar_count, var_len);
                char *env_val = get_env_value(env, var_name);
                result = strjoin_and_free(result, env_val ? env_val : "");
                // free(var_name);
                i += dollar_count + var_len;
            }
            else
            {
                // Expansion vide pour pair de dollars ou pas de variable après
                i += dollar_count;
            }
        }
        else
        {
            // Caractère normal
            char tmp[2] = {input[i], 0};
            result = strjoin_and_free(result, tmp);
            i++;
        }
    }
    return result;
}

int count_dollars(const char *str, int index)
{
    int count = 0;
    while (str[index] == '$')
    {
        count++;
        index++;
    }
    return count;
}

// Prétraite la string selon la logique :
// - groupe impair de dollars -> un seul $
// - groupe pair de dollars  -> supprime tout
// - edge case $"USER" devient USER (sans dollar ni quotes)
char *reduce_odd_dollars_and_handle_edge(const char *str)
{
    int i = 0;
    char *result = ft_strdup("");
    while (str[i])
    {
        // Edge case $"USER"
        if (str[i] == '$' && str[i+1] == '"' && str[i+2])
        {
            int j = i + 2;
            int start = j;
            while (str[j] && str[j] != '"')
                j++;
            if (str[j] == '"')
            {
                char *content = ft_strndup(str + start, j - start);
                result = strjoin_and_free(result, content);
                i = j + 1;
                continue;
            }
        }
        if (str[i] == '$')
        {
            int count = count_dollars(str, i);
            if (count % 2 == 1)
                result = strjoin_and_free(result, ft_strdup("$"));
            // sinon on ne met rien
            i += count;
        }
        else
        {
            char tmp[2] = { str[i], 0 };
            result = strjoin_and_free(result, ft_strdup(tmp));
            i++;
        }
    }
    return result;
}