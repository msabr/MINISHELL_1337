#include "../../minishell.h"

// Fonction principale d'expansion sur la liste de tokens
void expand_token_list_v2(t_token *tokens, t_env **env, int last_status)
{
    t_token *curr = tokens;
    while (curr)
    {
        if (curr->type == TOKEN_SQUOTE)
        {
            char *tmp = remove_quotes(curr->value, '\'');
            free(curr->value);
            curr->value = tmp;
        }
        else if (curr->type == TOKEN_DQUOTE)
        {
            int s = count_dollars(curr->value, 0);
            printf("s %d\n", s);
            if (s % 2 != 0)
            {
                char *expanded = expand_quoted_string(curr->value, env, last_status);
                char *cleaned = remove_quotes(expanded, '\"');
                free(curr->value);
                free(expanded);
                curr->value = cleaned;
            }
        }
        else if (curr->type == TOKEN_WORD || curr->type == TOKEN_VARIABLE)
        {
            char *expanded = expand_unquoted_string(curr->value, env, last_status);
            char *cleaned = clean_expansion_result(expanded);
            free(curr->value);
            free(expanded);
            curr->value = cleaned;
        }
        curr = curr->next;
    }
}

// Vérifie si un token nécessite une expansion
bool needs_expansion(t_token *token)
{
    if (!token || !token->value)
        return false;
    
    // Si le token était initialement quoté, on peut le détecter avec le champ quoted
    if (token->quoted && (token->type == TOKEN_SQUOTE || token->type == TOKEN_DQUOTE))
        return true;
        
    return (strchr(token->value, '$') || strchr(token->value, '\'') || strchr(token->value, '\"'));
}

// Expansion d'un token unique
void expand_single_token(t_token *token, t_env **env, int last_status)
{
    char *expanded;

    if (token->type == TOKEN_SQUOTE)
        expanded = remove_quotes(token->value, '\'');
    else if (token->type == TOKEN_DQUOTE)
        expanded = expand_quoted_string(token->value, env, last_status);
    else
        expanded = expand_unquoted_string(token->value, env, last_status);

    free(token->value);
    token->value = clean_expansion_result(expanded);
}