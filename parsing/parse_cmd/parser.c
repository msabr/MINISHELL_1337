/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 01:56:37 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 02:56:07 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../minishell.h"
#include "../parsing.h"

static int	process_split_args(t_cmd *current, char *arg)
{
	char	**split_args;
	int		idx;

	split_args = ft_split_space(arg);
	idx = 0;
	while (split_args && split_args[idx])
	{
		if (!add_argument(&current->args, split_args[idx]))
			return (0);
		idx++;
	}
	return (1);
}

static int	process_export_logic(t_cmd *current, char *arg, int was_quoted)
{
	if (current->args && current->args[0]
		&& strcmp(current->args[0], "export") == 0 && was_quoted == 0)
	{
		if (!add_argument(&current->args, arg))
			return (0);
		return (1);
	}
	else if (current->args && current->args[0]
		&& strcmp(current->args[0], "export") == 0)
	{
		return (process_split_args(current, arg));
	}
	return (-1);
}

int	process_ar_tk_helper(t_token **ptok, t_cmd *curr, char *arg, t_token *tok)
{
	if (!tok->quoted && arg[0] == '\0'
		&& (curr->args == NULL || curr->args[0] == NULL))
		return (1);
	if (tok->expended && !tok->quoted && is_only_spaces(arg))
	{
		*ptok = tok->next;
		return (1);
	}
	if (tok->quoted || arg[0] == '\0' || is_only_spaces(arg))
	{
		if (!add_argument(&curr->args, arg))
			return (0);
		return (2);
	}
	return (-1);
}

static int	process_argument_token(t_token **ptok, t_cmd *current)
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

static int	parse_tokens_loop(t_token *tok, t_cmd **cmds)
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
