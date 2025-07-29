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

#include "../parsing.h"

t_token	*lst_new_token(const char *val, t_token_type type, bool space, int expd)
{
	t_token	*tok;

	tok = ft_malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(val);
	tok->type = type;
	tok->space_after = space;
	tok->quoted = 0;
	tok->expended = expd;
	tok->next = NULL;
	tok->prev = NULL;
	return (tok);
}

void	lst_add_back(t_token **list, t_token *new)
{
	t_token	*tmp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}

void	remove_hel(t_token **tok, t_token **prev, t_token *curr, t_token *next)
{
	if (curr->type == TOKEN_WORD && (!curr->value || curr->value[0] == '\0'))
	{
		if (*prev && is_redir((*prev)->type))
			*prev = curr;
		if (*prev)
			(*prev)->next = next;
		else
			*tok = next;
	}
	else
		*prev = curr;
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
		remove_hel(tokens, &prev, curr, next);
		curr = next;
	}
}
