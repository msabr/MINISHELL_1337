/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_tool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:27:43 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/28 19:28:09 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	error_syntax(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token && *token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	ft_set_status(258);
}

int	check_unclosed_quote(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input && input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
	{
		ft_set_status(258);
		return (1);
	}
	return (0);
}

const char	*token_repr(t_token *tok)
{
	if (!tok || tok->type == TOKEN_EOF)
		return ("newline");
	if (tok->type == TOKEN_PIPE)
		return ("|");
	if (tok->type == TOKEN_REDIR_IN)
		return ("<");
	if (tok->type == TOKEN_REDIR_OUT)
		return (">");
	if (tok->type == TOKEN_REDIR_APPEND)
		return (">>");
	if (tok->type == TOKEN_HEREDOC)
		return ("<<");
	if (tok->type == TOKEN_WORD || tok->type == TOKEN_VARIABLE)
		return (tok->value);
	if (tok->value)
		return (tok->value);
	return ("");
}

int	is_valid_word(char *value)
{
	if (!value)
		return (1);
	while (*value)
	{
		if (*value == '&' || *value == '(' || *value == ')' || *value == ';')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token  `", 2);
			ft_putchar_fd(*value, 2);
			ft_putstr_fd("'\n", 2);
			ft_set_status(258);
			return (0);
		}
		value++;
	}
	return (1);
}
