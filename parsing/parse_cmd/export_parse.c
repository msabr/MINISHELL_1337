/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:40:14 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 02:18:19 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	lst_remove_token(t_token **head, t_token *node)
{
	if (!node)
		return ;
	if (node->prev)
		node->prev->next = node->next;
	else
		*head = node->next;
	if (node->next)
		node->next->prev = node->prev;
}

void	lst_insert_before(t_token **head, t_token *node, t_token *new_token)
{
	if (!node || !new_token)
		return ;
	new_token->next = node;
	new_token->prev = node->prev;
	if (node->prev)
		node->prev->next = new_token;
	else
		*head = new_token;
	node->prev = new_token;
}

int	is_assignment_word(const char *str)
{
	int	i;

	if (!str || !ft_isalpha(str[0]))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	if (str[i] == '=')
		return (1);
	if (str[i] == '+' && str[i + 1] == '=')
		return (1);
	return (0);
}

t_token	*split_and_insert(t_token **tokens, t_token *current)
{
	char		**split;
	int			i;
	t_token		*new_token;
	t_token		*tmp;

	if (ft_strchr(current->value, ' '))
		split = ft_split(current->value, ' ');
	else
		split = ft_split(current->value, '\t');
	if (!split)
		return (NULL);
	i = 0;
	while (split[i])
	{
		new_token = lst_new_token(ft_strdup(split[i]), TOKEN_WORD, true, 0);
		if (!new_token)
			return (NULL);
		lst_insert_before(tokens, current, new_token);
		i++;
	}
	tmp = current;
	current = current->next;
	lst_remove_token(tokens, tmp);
	return (current);
}

void	field_split_tokens(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD
			&& current->value
			&& current->quoted == 0
			&& !is_assignment_word(current->value)
			&& (ft_strchr(current->value, ' ')
				|| ft_strchr(current->value, '\t')))
		{
			current = split_and_insert(tokens, current);
			continue ;
		}
		current = current->next;
	}
}
