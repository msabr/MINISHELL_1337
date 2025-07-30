/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:56:37 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 10:39:51 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	process_argument_token(t_token **ptok, t_cmd *current)
{
	t_token	*tok;
	char	*arg;
	int		export_result;
	int		helper_ret;

	tok = *ptok;
	arg = merge_argument(ptok);
	if (!arg)
		return (0);
	helper_ret = process_ar_tk_helper(ptok, current, arg, tok);
	if (helper_ret == 1)
		return (1);
	if (helper_ret == 0)
		return (0);
	if (helper_ret == 2)
		arg = NULL;
	export_result = process_export_logic(current, arg, tok->quoted);
	if (export_result == 1)
		return (1);
	if (export_result == 0)
		return (0);
	if (!process_split_args(current, arg))
		return (0);
	return (1);
}

int	parse_tokens_iteration(t_token **ptok, t_cmd **current, t_cmd **cmds)
{
	if (!*current)
	{
		*current = new_command();
		if (!add_command(cmds, *current))
			return (0);
	}
	if (is_arg_token(*ptok))
	{
		if (!process_argument_token(ptok, *current))
			return (0);
		return (2);
	}
	if (is_redir((*ptok)->type))
	{
		if (!handle_redir(*current, ptok))
			return (0);
		return (2);
	}
	if ((*ptok)->type == TOKEN_PIPE)
	{
		*current = NULL;
		*ptok = (*ptok)->next;
		return (2);
	}
	return (*ptok = (*ptok)->next, 1);
}

int	parse_tokens_loop(t_token *tok, t_cmd **cmds)
{
	t_cmd	*current;
	int		iter_ret;

	current = NULL;
	while (tok && tok->type != TOKEN_EOF)
	{
		iter_ret = parse_tokens_iteration(&tok, &current, cmds);
		if (iter_ret == 0)
			return (0);
		if (iter_ret == 2)
			continue ;
	}
	return (1);
}

t_cmd	*parse_tokens_to_cmd2s(t_token *tokens)
{
	t_cmd	*cmds;

	cmds = NULL;
	remove_empty_word_tokens(&tokens);
	if (!tokens || (tokens->type == TOKEN_EOF && !tokens->next))
		return (NULL);
	if (!parse_tokens_loop(tokens, &cmds))
		return (NULL);
	return (cmds);
}
