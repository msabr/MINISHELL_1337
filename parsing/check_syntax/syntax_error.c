/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 19:27:18 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/28 19:27:18 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

// ---> debut de fonction diviser 
static int	check_input_and_quotes(const char *input)
{
	if (!input || !*input)
	{
		ft_set_status(0);
		return (0);
	}
	if (check_unclosed_quote(input))
		return (1);
	return (-1);
}

static int	check_pipe_or_redir_start(t_token *curr)
{
	if (curr && curr->type == TOKEN_PIPE)
		return (error_syntax(token_repr(curr)), 1);
	if (curr && is_redir(curr->type) && curr->type != TOKEN_HEREDOC)
	{
		if (!curr->next || !is_arg_token(curr->next))
			return (error_syntax(token_repr(curr)), 1);
	}
	return (0);
}

static int	check_token_errors(t_token *curr)
{
	while (curr && curr->type != TOKEN_EOF)
	{
		if (curr->type == TOKEN_WORD && curr->value)
		{
			if (!curr->expended && !is_valid_word(curr->value))
				return (1);
		}
		if (curr->type == TOKEN_PIPE && curr->next
			&& curr->next->type == TOKEN_PIPE)
			return (error_syntax("|"), 1);
		if (curr->type == TOKEN_PIPE
			&& (!curr->next || curr->next->type == TOKEN_EOF))
			return (error_syntax("newline"), 1);
		if (is_redir(curr->type) && curr->next
			&& is_redir(curr->next->type))
			return (error_syntax(token_repr(curr->next)), 1);
		if (curr->type == TOKEN_HEREDOC)
		{
			if (!curr->next || curr->next->type == TOKEN_EOF
				|| !curr->next->value || curr->next->value[0] == '\0')
				return (error_syntax("newline"), 1);
		}
		curr = curr->next;
	}
	return (0);
}

static int	is_only_redir_and_args(t_token *tokens)
{
	t_token	*curr;
	int		only_redir;

	only_redir = 1;
	curr = tokens;
	while (curr && curr->type != TOKEN_EOF)
	{
		if (!is_redir(curr->type) && !is_arg_token(curr))
			only_redir = 0;
		curr = curr->next;
	}
	return (only_redir);
}

int	check_syntax_errors(t_token *tokens, const char *input)
{
	t_token	*curr;
	int		ret;

	ret = check_input_and_quotes(input);
	if (ret != -1)
		return (ret);
	curr = tokens;
	ret = check_pipe_or_redir_start(curr);
	if (ret)
		return (1);
	ret = check_token_errors(curr);
	if (ret)
		return (1);
	if (is_only_redir_and_args(tokens))
		return (0);
	return (0);
}
