/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:26:57 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 11:41:17 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

char	*ft_strndup(const char *src, size_t n)
{
	char	*dst;
	size_t	i;

	dst = (char *)ft_malloc(n + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

bool	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

bool	is_double_operator(const char *s)
{
	return (!ft_strncmp(s, ">>", 2) || !ft_strncmp(s, "<<", 2));
}

t_token_type	get_operator_type(const char *s)
{
	if (!ft_strncmp(s, "|", 1))
		return (TOKEN_PIPE);
	if (!ft_strncmp(s, ">>", 2))
		return (TOKEN_REDIR_APPEND);
	if (!ft_strncmp(s, "<<", 2))
		return (TOKEN_HEREDOC);
	if (!ft_strncmp(s, "<", 1))
		return (TOKEN_REDIR_IN);
	if (!ft_strncmp(s, ">", 1))
		return (TOKEN_REDIR_OUT);
	return (TOKEN_WORD);
}
