/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   store_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 19:35:14 by marvin            #+#    #+#             */
/*   Updated: 2025/06/26 19:35:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void insert_back(t_data *list, t_token *token)
{
	t_token *last = *list;

	if (!list || !token)
		return;

	if (!*list)
	{
		*list = token;
		token->prev = NULL;
		token->next = NULL;
	}
	else
	{
		while (last->next)
			last = last->next;
		last->next = token;
		token->prev = last;
		token->next = NULL;
	}
}

void insert_in_pos(t_token **list,t_token token ,t_token target)
{
	if (!token || !target || !list)
		return;
	if (target->prev)
		target->prev->next = token;
	else
		*list = token;
	token->prev = target->prev;
	token ->next = target;
	target->prev = token;

}
void	lst_remove_token(t_token **head, t_token *token)
{
	if (!head || !*head || !token)
		return ;

	if (token->prev)
		token->prev->next = token->next;
	else
		*head = token->next;

	if (token->next)
		token->next->prev = token->prev;

	free(token->value);
	free(token);
}

