/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:36:22 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 12:54:32 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	is_quote_closed(const char *input, size_t i, char quote)
{
	while (input[i])
	{
		if (input[i] == quote)
			return (1);
		if (quote == '"' && input[i] == '\\' && input[i + 1])
			i += 2;
		else
			i++;
	}
	return (0);
}

size_t	variable_length(const char *input, size_t i)
{
	size_t	len;

	len = 1;
	if (input[i + 1] == '?')
		return (2);
	while (input[i + len])
	{
		if ((input[i + len] >= 'a' && input[i + len] <= 'z')
			|| (input[i + len] >= 'A' && input[i + len] <= 'Z')
			|| (input[i + len] >= '0' && input[i + len] <= '9')
			|| input[i + len] == '_')
			len++;
		else
			break ;
	}
	return (len);
}

void	handle_word(const char *input, size_t *i, t_token **head)
{
	size_t	len;
	char	*val;
	bool	space;

	len = 0;
	while (input[*i + len] && !is_operator(input[*i + len])
		&& input[*i + len] != '\'' && input[*i + len] != '"'
		&& !is_whitespace(input[*i + len]))
	{
		if (input[*i + len] == '\\' && input[*i + len + 1])
			len += 2;
		else
			len++;
	}
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	if (val && val[0] != '\0')
		add_token(head, val, TOKEN_WORD, space, 0);
	*i += len;
}

void	add_token(t_token **head, const char *val, t_token_type type, bool space, int expneded)
{
	t_token	*new;

	new = lst_new_token(val, type, space, expneded);
	lst_add_back(head, new);
}

void	add_token_quoted(t_token **head, const char *val,
			t_token_type type, bool space, int expneded)
{
	t_token	*new;

	new = lst_new_token(val, type, space, expneded);
	new->quoted = 1;
	lst_add_back(head, new);
}
