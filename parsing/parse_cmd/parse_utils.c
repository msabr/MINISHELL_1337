/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 00:32:10 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 02:18:32 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	is_arg_token(t_token *tok)
{
	return (tok && (tok->type == TOKEN_WORD || tok->type == TOKEN_VARIABLE || \
		tok->type == TOKEN_DQUOTE || tok->type == TOKEN_SQUOTE));
}

int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIR_IN || t == TOKEN_REDIR_OUT
		|| t == TOKEN_REDIR_APPEND || t == TOKEN_HEREDOC);
}

size_t	arg_total_len(t_token *tok)
{
	size_t	len;

	len = 0;
	while (is_arg_token(tok) && !tok->space_after)
	{
		len += ft_strlen(tok->value);
		tok = tok->next;
	}
	if (is_arg_token(tok))
		len += ft_strlen(tok->value);
	return (len);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}

int	add_argument(char ***args, char *new_arg)
{
	int		argc;
	char	**new_args ;
	int		i;

	argc = count_args(*args);
	new_args = ft_malloc(sizeof(char *) * (argc + 2));
	i = 0;
	if (!new_args)
		return (0);
	while (i < argc)
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[argc] = new_arg;
	new_args[argc + 1] = NULL;
	*args = new_args;
	return (1);
}
