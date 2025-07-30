/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handler2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 06:03:24 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 07:18:10 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

t_tk_args	new_tk_args(const char *val, t_token_type type, bool spc, int expd)
{
	t_tk_args	args;

	args.val = val;
	args.type = type;
	args.space = spc;
	args.expneded = expd;
	return (args);
}

static void	get_quote_value(const char *inpt, size_t *i, char qte, char **val)
{
	size_t	start;
	size_t	len;

	start = *i;
	len = 0;
	while (inpt[*i] && inpt[*i] != qte)
	{
		if (qte == '"' && inpt[*i] == '\\' && inpt[*i + 1])
			*i += 2;
		else
			(*i)++;
		len++;
	}
	*val = ft_strndup(inpt + start, len);
}

void	handle_quote_token(const char *inpt, size_t *i, char qte, t_token **hd)
{
	char			*val;
	bool			space;
	t_token_type	type;
	t_tk_args		args;

	val = NULL;
	get_quote_value(inpt, i, qte, &val);
	if (inpt[*i + 1] == '\0' || is_whitespace(inpt[*i + 1]))
		space = 1;
	else
		space = 0;
	if (qte == '\'')
		type = TOKEN_SQUOTE;
	else
		type = TOKEN_DQUOTE;
	args = new_tk_args(val, type, space, 1);
	add_token_quoted(hd, &args);
	if (inpt[*i])
		(*i)++;
}

void	handle_quote(const char *input, size_t *i, t_token **head)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	if (!is_quote_closed(input, *i, quote))
	{
		error_syntax("unclosed quote");
		return ;
	}
	handle_quote_token(input, i, quote, head);
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
