/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:28:59 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 07:14:56 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

static size_t	get_word_len(const char *input, size_t i)
{
	size_t	len;

	len = 0;
	while (input[i + len] && !is_operator(input[i + len])
		&& input[i + len] != '\''
		&& input[i + len] != '"'
		&& !is_whitespace(input[i + len]))
	{
		if (input[i + len] == '\\' && input[i + len + 1])
			len += 2;
		else
			len++;
	}
	return (len);
}

void	handle_word(const char *input, size_t *i, t_token **head)
{
	size_t		len;
	char		*val;
	bool		space;
	t_tk_args	args;

	len = get_word_len(input, *i);
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	if (val && val[0] != '\0')
	{
		args = new_tk_args(val, TOKEN_WORD, space, 0);
		add_token(head, &args);
	}
	*i += len;
}

void	handle_variable(const char *input, size_t *i, t_token **head)
{
	size_t		len;
	char		*val;
	bool		space;
	t_tk_args	args;

	len = variable_length(input, *i);
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	args = new_tk_args(val, TOKEN_VARIABLE, space, 1);
	add_token(head, &args);
	*i += len;
}

void	handle_operator(const char *input, size_t *i, t_token **head)
{
	size_t			len;
	char			*val;
	bool			space;
	t_token_type	type;
	t_tk_args		args;

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
	args = new_tk_args(val, type, space, 0);
	add_token(head, &args);
	*i += len;
}

t_token	*lexer(const char *input)
{
	t_token		*head;
	size_t		i;
	t_tk_args	args;

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
	args = new_tk_args("", TOKEN_EOF, 0, 0);
	add_token(&head, &args);
	return (head);
}
