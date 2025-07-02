/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 01:44:29 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/01 18:08:28 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

 void	add_token(t_token **head, const char *val, t_token_type type, bool space)
{
	t_token	*new;

	new = lst_new_token(val, type, space);
	lst_add_back(head, new);
}

 size_t	parse_quote(const char *input, size_t i, t_token **head)
{
	char	quote;
	size_t	start;
	char	*val;
	bool	space;

	quote = input[i];
	i++;
	start = i;
	while (input[i] && input[i] != quote)
	{
		if (quote == '"' && input[i] == '\\' && input[i + 1])
			i += 2;
		else
			i++;
	}
	if (!input[i])
	{
		write(2, "minishell: syntax error: unclosed quote\n", 39);
		return (i);
	}
	val = ft_strndup(input + start, i - start);
	space = 0;
	if (input[i + 1] == '\0' || ft_isspace(input[i + 1]))
		space = 1;
	if (quote == '\'')
		add_token(head, val, TOKEN_SQUOTE, space);
	else
		add_token(head, val, TOKEN_DQUOTE, space);
	return (i + 1);
}

 size_t	parse_variable(const char *input, size_t i, t_token **head)
{
	size_t	start;
	char	*val;
	bool	space;

	start = i;
	i++;
	if (input[i] == '?')
		i++;
	else
	{
		while (input[i])
		{
			if ((input[i] >= 'a' && input[i] <= 'z')
				|| (input[i] >= 'A' && input[i] <= 'Z')
				|| (input[i] >= '0' && input[i] <= '9')
				|| input[i] == '_')
				i++;
			else
				break;
		}
	}
	val = ft_strndup(input + start, i - start);
	space = 0;
	if (input[i] == '\0' || ft_isspace(input[i]))
		space = 1;
	add_token(head, val, TOKEN_VARIABLE, space);
	return (i);
}

 size_t	parse_operator(const char *input, size_t i, t_token **head)
{
	t_token_type	type;
	size_t			len;
	char			*val;
	bool			space;

	type = get_operator_type(input + i);
	len = 1;
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		len = 2;
	val = ft_strndup(input + i, len);
	space = 0;
	if (input[i + len] == '\0' || ft_isspace(input[i + len]))
		space = 1;
	add_token(head, val, type, space);
	return (i + len);
}

//  size_t	parse_word(const char *input, size_t i, t_token **head)
// {
// 	size_t	start;
// 	char	*val;
// 	bool	space;

// 	start = i;
// 	while (input[i]
// 		&& !is_operator(input[i])
// 		&& input[i] != '\''
// 		&& input[i] != '"'
// 		&& input[i] != '$'
// 		&& !ft_isspace(input[i]))
// 	{
// 		if (input[i] == '\\' && input[i + 1])
// 			i += 2;
// 		else
// 			i++;
// 	}
// 	val = ft_strndup(input + start, i - start);
// 	space = 0;
// 	if (input[i] == '\0' || ft_isspace(input[i]))
// 		space = 1;
// 	add_token(head, val, TOKEN_WORD, space);
// 	free(val);
// 	return (i);
// }


size_t	parse_word(const char *input, size_t i, t_token **head)
{
	size_t	start;
	char	*val;
	bool	space;

	start = i;
	while (input[i]
		&& !is_operator(input[i])
		&& input[i] != '\''
		&& input[i] != '"'
		/* && input[i] != '$' */ 
		&& !ft_isspace(input[i]))
	{
		if (input[i] == '\\' && input[i + 1])
			i += 2;
		else
			i++;
	}
	val = ft_strndup(input + start, i - start);
	space = 0;
	if (input[i] == '\0' || ft_isspace(input[i]))
		space = 1;
	add_token(head, val, TOKEN_WORD, space);
	return (i);
}

size_t	skip_whitespace(const char *input, size_t i)
{
	while (input[i] && ft_isspace(input[i]))
		i++;
	return (i);
}

t_token	*lexer2(const char *input)
{
	t_token	*head;
	size_t	i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		i = skip_whitespace(input, i);
		if (!input[i])
			break;
		if (is_operator(input[i]))
			i = parse_operator(input, i, &head);
		else if (input[i] == '\'')
			i = parse_quote(input, i, &head);
		else if (input[i] == '"')
			i = parse_quote(input, i, &head);
		else if (input[i] == '$')
			i = parse_variable(input, i, &head);
		else
			i = parse_word(input, i, &head);
	}
	add_token(&head, NULL, TOKEN_EOF, 0);
	return (head);
}
