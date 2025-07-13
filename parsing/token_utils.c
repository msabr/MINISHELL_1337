/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:32:51 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/01 17:02:56 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *lst_new_token(const char *value, t_token_type type, bool space_after,int quoted)
{
    t_token *tok = ft_malloc(sizeof(t_token));
    if (!tok) return NULL;
    tok->value = ft_strdup(value);
    tok->type = type;
    tok->space_after = space_after;
    tok->quoted = quoted;
    tok->next = NULL;
    tok->prev = NULL;
    return tok;
}
void lst_add_back(t_token **list, t_token *new)
{
    t_token *tmp;
    if (!*list)
    {
        *list = new;
        return;
    }
    tmp = *list; 
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    new->prev = tmp;
}
bool is_operator(char c) {
    return (c == '|' || c == '<' || c == '>');
}

bool is_double_operator(const char *s) {
    return (!ft_strncmp(s, ">>", 2) || !ft_strncmp(s, "<<", 2));
}

// t_token_type get_operator_type(const char *s) {
//     if (!ft_strncmp(s, "|", 1)) return TOKEN_PIPE;
//     if (!ft_strncmp(s, ">>", 2)) return TOKEN_REDIR_APPEND;
//     if (!ft_strncmp(s, "<<", 2)) return TOKEN_HEREDOC;
//     if (!ft_strncmp(s, "<", 1)) return TOKEN_REDIR_IN;
//     if (!ft_strncmp(s, ">", 1)) return TOKEN_REDIR_OUT;
//     return TOKEN_WORD;
// }

void free_token_list(t_token *head) {
    t_token *tmp;
    while (head) {
        tmp = head->next;
        if (head->value)
            free(head->value);
        free(head);
        head = tmp;
    }
}
