/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalid <khalid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:00:00 by khalid            #+#    #+#             */
/*   Updated: 2025/06/28 00:00:00 by khalid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *ft_lstnew_token(char *value, t_type type, int a_sp)
{
    t_token *new_token;

    new_token = malloc(sizeof(t_token));
    if (!new_token)
        return (NULL);
    
    new_token->value = value;
    new_token->type = type;
    new_token->a_sp = a_sp;
    new_token->next = NULL;
    
    return (new_token);
}


void ft_lstadd_back_token(t_token **lst, t_token *new)
{
    t_token *last;

    if (!lst || !new)
        return;
    
    if (!*lst)
    {
        *lst = new;
        return;
    }
    
    last = ft_last_token(lst);
    last->next = new;
}


t_token *ft_last_token(t_token **lst)
{
    t_token *current;

    if (!lst || !*lst)
        return (NULL);
    
    current = *lst;
    while (current->next)
        current = current->next;
    
    return (current);
}

void ft_free_tokens(t_token *tokens)
{
    t_token *current;
    t_token *next;

    current = tokens;
    while (current)
    {
        next = current->next;
        if (current->value)
            free(current->value);
        free(current);
        current = next;
    }
}
