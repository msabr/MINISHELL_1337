#include "../minishell.h"

/* ========== FONCTIONS PRINCIPALES ========== */

// Fonction principale d'expansion
void expand_token_list_v2(t_token *tokens, t_env **env, int last_status)
{
    t_token *curr;
    
    curr = tokens;
    while (curr)
    {
        if (needs_expansion(curr))
        {
            expand_single_token(curr, env, last_status);
        }
        curr = curr->next;
    }
}

// Détermine si un token nécessite une expansion
bool needs_expansion(t_token *token)
{
    if (!token || !token->value)
        return false;
    
    // Variables et mots qui contiennent des $ nécessitent expansion
    if (token->type == TOKEN_VARIABLE || token->type == TOKEN_WORD)
    {
        if (ft_strchr(token->value, '$'))
            return true;
    }
    
    // Double quotes nécessitent expansion
    if (token->type == TOKEN_DQUOTE)
        return true;
    
    // Single quotes nécessitent suppression des quotes
    if (token->type == TOKEN_SQUOTE)
        return true;
    
    return false;
}

// Expansion d'un token selon son type
void expand_single_token(t_token *token, t_env **env, int last_status)
{
    char *new_value;
    
    if (token->type == TOKEN_SQUOTE)
    {
        // Simple quotes : pas d'expansion, juste supprimer les quotes
        new_value = remove_quotes(token->value, '\'');
        token->type = TOKEN_WORD;
    }
    else if (token->type == TOKEN_DQUOTE)
    {
        // Double quotes : expansion puis suppression des quotes
        new_value = expand_quoted_string(token->value, env, last_status);
        token->type = TOKEN_WORD;
    }
    else
    {
        // Mot normal ou variable : expansion directe
        new_value = expand_unquoted_string(token->value, env, last_status);
    }
    
    if (new_value)
    {
        free(token->value);
        token->value = new_value;
    }
}

/* ========== GESTION DES QUOTES ========== */

// Fonction utilitaire pour supprimer les quotes
char *remove_quotes(const char *str, char quote_char)
{
    char    *result;
    int     len;
    int     i;
    int     j;
    
    if (!str)
        return NULL;
    
    len = ft_strlen(str);
    if (len < 2 || str[0] != quote_char || str[len - 1] != quote_char)
        return ft_strdup(str);
    
    result = ft_calloc(len - 1, sizeof(char));
    if (!result)
        return NULL;
    
    j = 0;
    for (i = 1; i < len - 1; i++)
    {
        result[j++] = str[i];
    }
    
    return result;
}

/* ========== FONCTIONS UTILITAIRES ========== */

// Compte le nombre de mots dans une chaîne
int count_words(const char *str)
{
    int count;
    int i;
    bool in_word;
    
    if (!str)
        return 0;
    
    count = 0;
    i = 0;
    in_word = false;
    
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
        {
            in_word = false;
        }
        else if (!in_word)
        {
            in_word = true;
            count++;
        }
        i++;
    }
    
    return count;
}

/* ========== GESTION DES ÉCHAPPEMENTS ========== */

// Échappements dans les double quotes
char *handle_escape_in_dquotes(const char *str, int *index)
{
    char    *result;
    int     i;
    
    i = *index;
    
    if (str[i] != '\\')
        return NULL;
    
    i++; // Skip backslash
    
    // Dans les double quotes, seuls certains caractères sont échappés
    if (str[i] == '"' || str[i] == '\\' || str[i] == '$' || str[i] == '`' || str[i] == '\n')
    {
        result = ft_calloc(2, sizeof(char));
        if (result)
        {
            result[0] = str[i];
            result[1] = '\0';
        }
        *index = i + 1;
    }
    else
    {
        // Backslash littéral suivi du caractère
        result = ft_calloc(3, sizeof(char));
        if (result)
        {
            result[0] = '\\';
            result[1] = str[i];
            result[2] = '\0';
        }
        *index = i + 1;
    }
    
    return result;
}

// Échappements hors quotes
char *handle_escape_unquoted(const char *str, int *index)
{
    char    *result;
    int     i;
    
    i = *index;
    
    if (str[i] != '\\')
        return NULL;
    
    i++; // Skip backslash
    
    if (str[i] == '\0')
    {
        // Backslash en fin de ligne
        result = ft_strdup("\\");
        *index = i;
    }
    else
    {
        // Échapper le caractère suivant
        result = ft_calloc(2, sizeof(char));
        if (result)
        {
            result[0] = str[i];
            result[1] = '\0';
        }
        *index = i + 1;
    }
    
    return result;
}

/* ========== EXPANSION DANS LES QUOTES ========== */

// Expansion dans les double quotes
char *expand_quoted_string(const char *str, t_env **env, int last_status)
{
    char    *result;
    char    *temp;
    int     i;
    int     len;
    
    result = ft_calloc(4096, sizeof(char));
    if (!result)
        return NULL;
    
    i = 1; // Skip opening quote
    len = 0;
    
    while (str[i] && str[i] != '"')
    {
        if (str[i] == '$')
        {
            temp = handle_dollar_expansion(str, &i, env, last_status);
            if (temp)
            {
                ft_strlcat(result, temp, 4096);
                len += ft_strlen(temp);
                free(temp);
            }
        }
        else if (str[i] == '\\')
        {
            // Gestion des échappements dans les double quotes
            temp = handle_escape_in_dquotes(str, &i);
            if (temp)
            {
                ft_strlcat(result, temp, 4096);
                len += ft_strlen(temp);
                free(temp);
            }
        }
        else
        {
            result[len++] = str[i++];
        }
    }
    
    result[len] = '\0';
    return result;
}

// Expansion sans quotes
char *expand_unquoted_string(const char *str, t_env **env, int last_status)
{
    char    *result;
    char    *temp;
    int     i;
    int     len;
    
    result = ft_calloc(4096, sizeof(char));
    if (!result)
        return NULL;
    
    i = 0;
    len = 0;
    
    while (str[i])
    {
        if (str[i] == '$')
        {
            temp = handle_dollar_expansion(str, &i, env, last_status);
            if (temp)
            {
                ft_strlcat(result, temp, 4096);
                len += ft_strlen(temp);
                free(temp);
            }
        }
        else if (str[i] == '\\')
        {
            // Gestion des échappements hors quotes
            temp = handle_escape_unquoted(str, &i);
            if (temp)
            {
                ft_strlcat(result, temp, 4096);
                len += ft_strlen(temp);
                free(temp);
            }
        }
        else
        {
            result[len++] = str[i++];
        }
    }
    
    result[len] = '\0';
    return result;
}

/* ========== EXPANSION DES VARIABLES ========== */

// Gestion centralisée des expansions $
char *handle_dollar_expansion(const char *str, int *index, t_env **env, int last_status)
{
    char    *result;
    int     i;
    
    i = *index;
    
    if (str[i] != '$')
        return NULL;
    
    i++; // Skip $
    
    if (str[i] == '?')
    {
        // Exit status
        result = ft_itoa(last_status);
        *index = i + 1;
    }
    else if (str[i] == '$')
    {
        // Process ID
        result = ft_itoa(getpid());
        *index = i + 1;
    }
    else if (str[i] == '0')
    {
        // Nom du shell
        result = ft_strdup("minishell");
        *index = i + 1;
    }
    else if (ft_isdigit(str[i]))
    {
        // Paramètres positionnels (non supportés dans minishell)
        result = ft_strdup("");
        *index = i + 1;
    }
    else if (str[i] == '{')
    {
        // Variable avec accolades ${VAR}
        result = expand_braced_variable(str, index, env, last_status);
    }
    else if (ft_isalpha(str[i]) || str[i] == '_')
    {
        // Variable d'environnement simple
        result = expand_env_variable(str, index, env);
    }
    else
    {
        // $ seul ou suivi d'un caractère spécial
        result = ft_strdup("$");
        *index = i;
    }
    
    return result;
}

// Expansion d'une variable d'environnement
char *expand_env_variable(const char *str, int *index, t_env **env)
{
    char    var_name[256];
    char    *value;
    int     i;
    int     j;
    
    i = *index;
    j = 0;
    
    // Extraire le nom de la variable
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && j < 255)
    {
        var_name[j++] = str[i++];
    }
    var_name[j] = '\0';
    
    *index = i;
    
    if (j == 0)
        return ft_strdup("$");
    
    // Chercher la valeur
    value = get_env_value(env, var_name);
    if (value)
        return ft_strdup(value);
    
    return ft_strdup(""); // Variable non trouvée = chaîne vide
}

// Expansion d'une variable avec accolades ${VAR}
char *expand_braced_variable(const char *str, int *index, t_env **env, int last_status)
{
    char    var_name[256];
    char    *value;
    int     i;
    int     j;
    
    i = *index;
    
    if (str[i] != '{')
        return ft_strdup("$");
    
    i++; // Skip {
    j = 0;
    
    // Extraire le nom de la variable
    while (str[i] && str[i] != '}' && j < 255)
    {
        var_name[j++] = str[i++];
    }
    var_name[j] = '\0';
    
    if (str[i] != '}')
    {
        // Accolade fermante manquante
        return ft_strdup("$");
    }
    
    *index = i + 1; // Skip }
    
    if (j == 0)
        return ft_strdup("");
    
    // Cas spéciaux pour les variables avec accolades
    if (ft_strcmp(var_name, "?") == 0)
    {
        // ${?} - exit status
        return ft_itoa(last_status);
    }
    else if (ft_strcmp(var_name, "$") == 0)
    {
        // ${$} - process ID
        return ft_itoa(getpid());
    }
    else if (ft_strcmp(var_name, "0") == 0)
    {
        // ${0} - nom du shell
        return ft_strdup("minishell");
    }
    else if (ft_isdigit(var_name[0]))
    {
        // ${1}, ${2}, etc. - paramètres positionnels
        return ft_strdup("");
    }
    
    // Variable d'environnement normale
    value = get_env_value(env, var_name);
    if (value)
        return ft_strdup(value);
    
    return ft_strdup("");
}

/* ========== FONCTIONS UTILITAIRES AVANCÉES ========== */

// Vérifie si une chaîne contient des caractères spéciaux
bool has_special_chars(const char *str)
{
    int i;
    
    if (!str)
        return false;
    
    i = 0;
    while (str[i])
    {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
            str[i] == '*' || str[i] == '?' || str[i] == '[' ||
            str[i] == ']' || str[i] == '{' || str[i] == '}')
            return true;
        i++;
    }
    return false;
}

// Divise une chaîne en mots (word splitting)
char **split_words(const char *str)
{
    char    **result;
    char    *word;
    int     count;
    int     i;
    int     start;
    
    if (!str)
        return NULL;
    
    // Compter les mots
    count = count_words(str);
    if (count == 0)
        return NULL;
    
    result = ft_calloc(count + 1, sizeof(char *));
    if (!result)
        return NULL;
    
    i = 0;
    start = 0;
    count = 0;
    
    while (str[i])
    {
        // Ignorer les espaces
        while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
            i++;
        
        if (!str[i])
            break;
        
        start = i;
        
        // Trouver la fin du mot
        while (str[i] && !(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'))
            i++;
        
        // Extraire le mot
        word = ft_substr(str, start, i - start);
        if (word)
            result[count++] = word;
    }
    
    result[count] = NULL;
    return result;
}

// Fonction pour nettoyer et valider l'expansion
char *clean_expansion_result(char *str)
{
    char    *result;
    int     len;
    
    if (!str)
        return NULL;
    
    len = ft_strlen(str);
    
    // Supprimer les espaces en début et fin si nécessaire
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t'))
        len--;
    
    result = ft_calloc(len + 1, sizeof(char));
    if (!result)
    {
        free(str);
        return NULL;
    }
    
    ft_strlcpy(result, str, len + 1);
    free(str);
    
    return result;
}