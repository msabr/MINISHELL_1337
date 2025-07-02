/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 01:44:29 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/02 16:47:18 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_token(t_token **head, const char *val,
			t_token_type type, bool space)
{
	t_token	*new;

	new = lst_new_token(val, type, space);
	lst_add_back(head, new);
}

size_t	word_length(const char *input, size_t i)
{
	size_t	len;

	len = 0;
	while (input[i + len] && !is_operator(input[i + len])
		&& input[i + len] != '\''
		&& input[i + len] != '"'
		&& !ft_isspace(input[i + len]))
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
	size_t	len;
	char	*val;
	bool	space;

	len = word_length(input, *i);
	val = ft_strndup(input + *i, len);
	space = 0;
	if (input[*i + len] == '\0' || ft_isspace(input[*i + len]))
		space = 1;
	add_token(head, val, TOKEN_WORD, space);
	free(val);
	*i += len;
}

void	handle_quote_error(void)
{
	write(2, "minishell: syntax error: unclosed quote\n", 39);
}

bool	is_quote_closed(const char *input, size_t i, char quote)
{
	while (input[i])
	{
		if (input[i] == quote)
			return (true);
		if (quote == '"' && input[i] == '\\' && input[i + 1])
			i += 2;
		else
			i++;
	}
	return (false);
}

size_t	get_quote_end(const char *input, size_t i, char quote)
{
	while (input[i] && input[i] != quote)
	{
		if (quote == '"' && input[i] == '\\' && input[i + 1])
			i += 2;
		else
			i++;
	}
	return (i);
}

void	handle_quote(const char *input, size_t *i, t_token **head)
{
	char	quote;
	size_t	start;
	char	*val;
	bool	space;

	quote = input[*i];
	(*i)++;
	start = *i;
	if (!is_quote_closed(input, start, quote))
		return (handle_quote_error());
	*i = get_quote_end(input, start, quote);
	val = ft_strndup(input + start, *i - start);
	space = 0;
	if (input[*i + 1] == '\0' || ft_isspace(input[*i + 1]))
		space = 1;
	if (quote == '\'')
		add_token(head, val, TOKEN_SQUOTE, space);
	else
		add_token(head, val, TOKEN_DQUOTE, space);
	(*i)++;
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

void	handle_variable(const char *input, size_t *i, t_token **head)
{
	size_t	len;
	char	*val;
	bool	space;

	len = variable_length(input, *i);
	val = ft_strndup(input + *i, len);
	space = 0;
	if (input[*i + len] == '\0' || ft_isspace(input[*i + len]))
		space = 1;
	add_token(head, val, TOKEN_VARIABLE, space);
	free(val);
	*i += len;
}

size_t	operator_length(const char *input, size_t i)
{
	t_token_type	type;

	type = get_operator_type(input + i);
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (2);
	return (1);
}

void	handle_operator(const char *input, size_t *i, t_token **head)
{
	size_t	len;
	char	*val;
	bool	space;
	t_token_type	type;

	type = get_operator_type(input + *i);
	len = operator_length(input, *i);
	val = ft_strndup(input + *i, len);
	space = 0;
	if (input[*i + len] == '\0' || ft_isspace(input[*i + len]))
		space = 1;
	add_token(head, val, type, space);
	free(val);
	*i += len;
}

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

t_token	*lexer2(const char *input)
{
	t_token	*head;
	size_t	i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (!input[i])
			break ;
		if (is_operator(input[i]))
			handle_operator(input, &i, &head);
		else if (input[i] == '\'')
			handle_quote(input, &i, &head);
		else if (input[i] == '"')
			handle_quote(input, &i, &head);
		else if (input[i] == '$')
			handle_variable(input, &i, &head);
		else
			handle_word(input, &i, &head);
	}
	add_token(&head, NULL, TOKEN_EOF, 0);
	return (head);
}
