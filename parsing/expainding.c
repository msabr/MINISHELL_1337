#include "../minishell.h"


void set_env_value(t_env **env_list, const char *key, const char *value) {
    t_env *current = *env_list;
    while (current) {
        if (ft_strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }
    // Si non trouvée, ajoute à la fin
    t_env *new = malloc(sizeof(t_env));
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->export_variable = false;
    new->next = *env_list;
    *env_list = new;
}

/**
 * Vérifie si la chaîne est de la forme VAR=val (affectation shell)
 */
bool is_assignment(const char *str) {
    int i = 0;
    if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
        return false;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    return (str[i] == '=' && i > 0);
}

/**
 * Expansion d'un mot (prend en compte quotes, $?, $VAR)
 */
char *expand_word(const char *word, t_env **env_list, int last_status) {
    char buf[4096];
    int i = 0, r = 0;
    while (word[i]) {
        if (word[i] == '\'') {
            buf[r++] = word[i++];
            while (word[i] && word[i] != '\'')
                buf[r++] = word[i++];
            if (word[i]) buf[r++] = word[i++];
        }
        else if (word[i] == '"') {
            buf[r++] = word[i++];
            while (word[i] && word[i] != '"') {
                if (word[i] == '$') {
                    if (word[i+1] == '?') {
                        r += sprintf(buf + r, "%d", last_status);
                        i += 2;
                    } else {
                        char var[256] = {0};
                        int j = 0;
                        int start = i+1;
                        if (ft_isalpha(word[start]) || word[start] == '_') {
                            while (word[start + j] && (ft_isalnum(word[start + j]) || word[start + j] == '_')) {
                                var[j] = word[start + j];
                                j++;
                            }
                        }
                        var[j] = '\0';
                        if (j > 0) {
                            char *val = get_env_value(env_list, var);
                            if (val) r += sprintf(buf + r, "%s", val);
                            i += 1 + j;
                        } else {
                            buf[r++] = word[i++];
                        }
                    }
                } else {
                    buf[r++] = word[i++];
                }
            }
            if (word[i]) buf[r++] = word[i++];
        }
        else if (word[i] == '$') {
            if (word[i+1] == '?') {
                r += sprintf(buf + r, "%d", last_status);
                i += 2;
            } else {
                char var[256] = {0};
                int j = 0;
                int start = i+1;
                if (ft_isalpha(word[start]) || word[start] == '_') {
                    while (word[start + j] && (ft_isalnum(word[start + j]) || word[start + j] == '_')) {
                        var[j] = word[start + j];
                        j++;
                    }
                }
                var[j] = '\0';
                if (j > 0) {
                    char *val = get_env_value(env_list, var);
                    if (val) r += sprintf(buf + r, "%s", val);
                    i += 1 + j;
                } else {
                    buf[r++] = word[i++];
                }
            }
        }
        else {
            buf[r++] = word[i++];
        }
    }
    buf[r] = '\0';
    return ft_strdup(buf);
}

/**
 * Expansion sur toute la liste de tokens (TOKEN_WORD, TOKEN_VARIABLE)
 */
void expand_token_list(t_token *tokens, t_env **env, int last_status) {
    for (t_token *curr = tokens; curr; curr = curr->next) {
        if (curr->type == TOKEN_WORD || curr->type == TOKEN_VARIABLE) {
            char *new_val = expand_word(curr->value, env, last_status);
            free(curr->value);
            curr->value = new_val;
        }
    }
}