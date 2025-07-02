/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expainding.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 04:38:52 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/01 18:07:50 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *get_env_var(const char *name, char **envp)
{
    size_t len = ft_strlen(name);
    for (int i = 0; envp[i]; i++) {
        if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
            return envp[i] + len + 1;
    }
    return NULL;
}

// Helper to append a string, reallocating as needed
static void append_str(char **dst, const char *src, size_t n)
{
    size_t oldlen = 0;

    if (src == NULL) 
        return;
    if (n == 0) 
        return;
    if (*dst) 
        oldlen = strlen(*dst);
    char *tmp = realloc(*dst, oldlen + n + 1);
    if (tmp == NULL) 
        exit(1);
    ft_memcpy(tmp + oldlen, src, n);
    tmp[oldlen + n] = '\0';
    *dst = tmp;
}
}

// Helper for tilde expansion at start of word
static bool is_tilde_expand(const char *s, size_t pos)
{
    // Only expand ~ if at pos 0, and followed by \0, /, or space
    return pos == 0 && s[0] == '~' &&
        (s[1] == '/' || s[1] == '\0');
}

// Core expansion function: input=raw token, returns malloc'd expanded string
char *expand_string(const char *input, char **envp, int last_status)
{
    char *out = NULL;
    size_t i = 0;
    size_t len = strlen(input);
    char quote = 0; // 0=none, ' or "
    bool escape = false;

    while (i < len) {
        char c = input[i];

        // 1. Handle quote context
        if (!escape && (c == '\'' || c == '"')) {
            // Toggle quote, or close if matching
            if (!quote) quote = c;
            else if (quote == c) quote = 0;
            i++;
            continue;
        }

        // 2. Handle escapes (backslash)
        if (!escape && c == '\\') {
            if (quote == '\'') { // In single quotes, backslash is literal
                append_str(&out, &c, 1);
                i++;
                continue;
            }
            escape = true; 
            i++ ;
            continue;
        }

        // 3. Handle tilde expansion at beginning (if not quoted)
        if (!quote && is_tilde_expand(input, i)) {
            char *home = get_env_var("HOME", envp);
            if (home) append_str(&out, home, strlen(home));
            else append_str(&out, "~", 1);
            i++;
            continue;
        }

        // 4. Handle variable expansion
        if (!escape && c == '$') {
            if (quote == '\'') { // No expansion in simple quotes
                append_str(&out, &c, 1);
                i++;
                continue;
            }
            i++;
            // Case: $?
            if (input[i] == '?') {
                char status[16];
                snprintf(status, sizeof(status), "%d", last_status);
                append_str(&out, status, strlen(status));
                i++;
                continue;
            }
            // Case: ${VAR}
            if (input[i] == '{') {
                i++;
                size_t var_start = i;
                while (input[i] && input[i] != '}')
                    i++;
                if (input[i] == '}') {
                    size_t vlen = i - var_start;
                    char varname[256] = {0};
                    strncpy(varname, input + var_start, vlen);
                    char *val = get_env_var(varname, envp);
                    if (val) append_str(&out, val, strlen(val));
                    i++; // skip }
                }
                continue;
            }
            // Case: $VAR or $1
            if (isalpha(input[i]) || input[i] == '_') {
                size_t var_start = i;
                while (isalnum(input[i]) || input[i] == '_')
                    i++;
                size_t vlen = i - var_start;
                char varname[256] = {0};
                strncpy(varname, input + var_start, vlen);
                char *val = get_env_var(varname, envp);
                if (val) append_str(&out, val, strlen(val));
                continue;
            }
            // Case: $1 etc.
            if (isdigit(input[i])) {
                char varname[3] = {input[i], 0};
                char *val = get_env_var(varname, envp);
                if (val) append_str(&out, val, strlen(val));
                i++;
                continue;
            }
            // Else: $ followed by other char, copy as is
            append_str(&out, "$", 1);
            continue;
        }

        // 5. Normal character or escaped
        append_str(&out, &c, 1);
        i++;
        escape = false;
    }
    return out ? out : strdup("");
}

// Example: expand all tokens in a list (except in SQUOTE)
void expand_token_list(t_token *head, char **envp, int last_status)
{
    for (t_token *tok = head; tok; tok = tok->next)
    {
        if (tok->type == TOKEN_SQUOTE)
            continue; // No expansion in simple quotes
        char *expanded = expand_string(tok->value, envp, last_status);
        tok->value = expanded;
    }
}