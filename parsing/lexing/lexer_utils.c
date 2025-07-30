/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:36:22 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 06:06:21 by kabouelf         ###   ########.fr       */
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

char	*extract_word_value(const char *input, size_t *i, size_t *len)
{
	*len = 0;
	while (input[*i + *len]
		&& !is_operator(input[*i + *len])
		&& input[*i + *len] != '\''
		&& input[*i + *len] != '"'
		&& !is_whitespace(input[*i + *len]))
	{
		if (input[*i + *len] == '\\' && input[*i + *len + 1])
			*len += 2;
		else
			(*len)++;
	}
	return (ft_strndup(input + *i, *len));
}

void	add_word_token(const char *input, size_t *i, t_token **head)
{
	size_t		len;
	char		*val;
	bool		space;
	t_tk_args	args;

	val = extract_word_value(input, i, &len);
	space = 0;
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	if (val && val[0] != '\0')
	{
		args = new_tk_args(val, TOKEN_WORD, space, 0);
		add_token(head, &args);
	}
	*i += len;
}

void	add_token(t_token **head, t_tk_args *args)
{
	t_token	*new;

	new = lst_new_token(args->val, args->type, args->space, args->expneded);
	lst_add_back(head, new);
}

void	add_token_quoted(t_token **head, t_tk_args *args)
{
	t_token	*new;

	new = lst_new_token(args->val, args->type, args->space, args->expneded);
	new->quoted = 1;
	lst_add_back(head, new);
}
