/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_herdoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:33:30 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 05:31:21 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	was_originally_quoted(t_token *token)
{
	if (!token)
		return (0);
	return (token->quoted
		&& (token->type == TOKEN_SQUOTE || token->type == TOKEN_DQUOTE));
}

void	expansion_handle_heredoc(t_token *curr)
{
	if (was_originally_quoted(curr))
	{
		if (curr->type == TOKEN_DQUOTE)
			curr->value = remove_dquotes(curr->value);
		else if (curr->type == TOKEN_SQUOTE)
			curr->value = remove_squotes(curr->value);
	}
	curr->type = TOKEN_WORD;
}

int	is_in_heredoc(t_token *token)
{
	t_token	*prev;

	prev = token->prev;
	while (prev && prev->type == TOKEN_WORD && prev->space_after == false)
		prev = prev->prev;
	while (prev && prev->type == TOKEN_WORD && prev->space_after == true)
		prev = prev->prev;
	if (prev && prev->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

char	*expand_heredoc_content(const char *str, t_env **env, const char *del)
{
	t_token	*token;
	char	*expended;

	if (ft_strchr(del, '\'') || ft_strchr(del, '"'))
		return (ft_strdup(str));
	token = lst_new_token(str, TOKEN_WORD, false, 0);
	expansion_all_tokens(token, *env);
	expended = ft_strdup(token->value);
	return (expended);
}
