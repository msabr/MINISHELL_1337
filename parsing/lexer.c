/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:50:27 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/01 18:07:57 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t skip_whitespace(const char *input, size_t i) {
    while (input[i] && isspace(input[i]))
        i++;
    return i;
}

size_t parse_word(const char *input, size_t i, t_token **head) {
    size_t start = i;
    while (input[i] && !isspace(input[i]) && !strchr("|<>$'\"", input[i]))
        i++;
    if (start < i) {
        char *word = ft_strndup(input + start, i - start);
        lst_add_back(head, lst_new_token(word, TOKEN_WORD, isspace(input[i])));
    }
    return i;
}

size_t parse_quote(const char *input, size_t i, t_token **head) {
    char quote = input[i++];
    size_t start = i;
    while (input[i] && input[i] != quote)
        i++;
    char *word = strndup(input + start, i - start);
    lst_add_back(head, lst_new_token(word,
        (quote == '\'') ? TOKEN_SQUOTE : TOKEN_DQUOTE,
        isspace(input[i+1])));
    if (input[i] == quote)
        i++;
    return i;
}

size_t parse_operator(const char *input, size_t i, t_token **head) {
    if (input[i] == '>' && input[i+1] == '>') {
        lst_add_back(head, lst_new_token(">>", TOKEN_REDIR_APPEND, isspace(input[i+2])));
        return i+2;
    } else if (input[i] == '<' && input[i+1] == '<') {
        lst_add_back(head, lst_new_token("<<", TOKEN_HEREDOC, isspace(input[i+2])));
        return i+2;
    } else if (input[i] == '|') {
        lst_add_back(head, lst_new_token("|", TOKEN_PIPE, isspace(input[i+1])));
        return i+1;
    } else if (input[i] == '<') {
        lst_add_back(head, lst_new_token("<", TOKEN_REDIR_IN, isspace(input[i+1])));
        return i+1;
    } else if (input[i] == '>') {
        lst_add_back(head, lst_new_token(">", TOKEN_REDIR_OUT, isspace(input[i+1])));
        return i+1;
    }
    return i;
}

size_t parse_variable(const char *input, size_t i, t_token **head) {
    size_t start = i++;
    if (isalpha(input[i]) || input[i] == '_') {
        while (isalnum(input[i]) || input[i] == '_')
            i++;
    } else if (input[i] == '?' || isdigit(input[i])) {
        i++;
    }
    char *var = strndup(input + start, i - start);
    lst_add_back(head, lst_new_token(var, TOKEN_VARIABLE, isspace(input[i])));
    return i;
}

// Main lexer
t_token *lexer(const char *input) {
    t_token *head = NULL;
    size_t i = 0;
    while (input[i]) {
        i = skip_whitespace(input, i);
        if (!input[i]) break;
        if (input[i] == '\'' || input[i] == '"')
            i = parse_quote(input, i, &head);
        else if (strchr("|<>", input[i]))
            i = parse_operator(input, i, &head);
        else if (input[i] == '$')
            i = parse_variable(input, i, &head);
        else
            i = parse_word(input, i, &head);
    }
    return head;
}
