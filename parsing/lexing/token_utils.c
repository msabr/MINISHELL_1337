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

#include "../../minishell.h"

t_token	*lst_new_token(const char *value, t_token_type type, bool space_after, int expended)
{
	t_token	*tok;

	tok = ft_malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(value);
	tok->type = type;
	tok->space_after = space_after;
	tok->quoted = 0;
	tok->expended = expended;
	tok->next = NULL;
	tok->prev = NULL;
	return (tok);
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


void	add_token(t_token **head, const char *val,
			t_token_type type, bool space,int expneded)
{
	t_token	*new;

	new = lst_new_token(val, type, space,expneded);
	lst_add_back(head, new);
}

void	add_token_quoted(t_token **head, const char *val,
			t_token_type type, bool space, int expneded)
{
	t_token	*new;

	new = lst_new_token(val, type, space,expneded);
	new->quoted = 1;
	lst_add_back(head, new);
}
void	remove_empty_word_tokens(t_token **tokens)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*next;

	curr = *tokens;
	prev = NULL;
	if (curr)
	{
		prev = curr;
		curr = curr->next;
	}
	while (curr)
	{
		next = curr->next;
		if (curr->type == TOKEN_WORD
			&& (!curr->value || curr->value[0] == '\0'))
		{
			if (prev && is_redir(prev->type))
				prev = curr;
			if (prev)
				prev->next = next;
			else
				*tokens = next;
		}
		else
			prev = curr;
		curr = next;
	}
}