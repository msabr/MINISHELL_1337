/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:28:59 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 11:40:44 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	handle_quote_token(const char *inpt, size_t *i, char qte, t_token **hd)
{
	size_t			start;
	char			*val;
	bool			space;
	t_token_type	type;

	start = *i;
	while (inpt[*i] && inpt[*i] != qte)
	{
		if (qte == '"' && inpt[*i] == '\\' && inpt[*i + 1])
			*i += 2;
		else
			(*i)++;
	}
	val = ft_strndup(inpt + start, *i - start);
	if (inpt[*i + 1] == '\0' || is_whitespace(inpt[*i + 1]))
		space = 1;
	else
		space = 0;
	if (qte == '\'')
		type = TOKEN_SQUOTE;
	else
		type = TOKEN_DQUOTE;
	add_token_quoted(hd, val, type, space, 1);
	if (inpt[*i])
		(*i)++;
}

static void	handle_quote(const char *input, size_t *i, t_token **head)
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

static void	handle_variable(const char *input, size_t *i, t_token **head)
{
	size_t	len;
	char	*val;
	bool	space;

	len = variable_length(input, *i);
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	add_token(head, val, TOKEN_VARIABLE, space, 1);
	*i += len;
}

static void	handle_operator(const char *input, size_t *i, t_token **head)
{
	size_t			len;
	char			*val;
	bool			space;
	t_token_type	type;

	type = get_operator_type(input + *i);
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		len = 2;
	else
		len = 1;
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	add_token(head, val, type, space, 0);
	*i += len;
}

t_token	*lexer(const char *input)
{
	t_token	*head;
	size_t	i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
		{
			while (input[i] && is_whitespace(input[i]))
				i++;
		}
		else if (is_operator(input[i]))
			handle_operator(input, &i, &head);
		else if (input[i] == '\'' || input[i] == '"')
			handle_quote(input, &i, &head);
		else if (input[i] == '$')
			handle_variable(input, &i, &head);
		else
			handle_word(input, &i, &head);
	}
	add_token(&head, "", TOKEN_EOF, 0, 0);
	return (head);
}
