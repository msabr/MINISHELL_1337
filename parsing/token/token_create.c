/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 19:22:21 by marvin            #+#    #+#             */
/*   Updated: 2025/06/26 19:22:21 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token    *token_create(t_type type, char *value)
{
    t_token	*new_token;
    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token) 
        return NULL;
    new_token->type = type;
    new_token->value = value;
    new_token->quoted = 0;
    new_token->next = NULL;
    new_token->prev = NULL;
    return (new_token);
}
