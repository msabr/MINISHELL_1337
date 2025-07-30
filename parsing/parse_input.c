/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 10:18:39 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 10:36:22 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*parse_input(char *input, t_env *env_list, int *status)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = lexer(input);
	expansion_all_tokens(tokens, env_list);
	field_split_tokens(&tokens);
	(void)env_list;
	merge_collapsed_tokens(tokens);
	if (check_syntax_errors(tokens, input))
	{
		*status = 258;
		return (NULL);
	}
	cmds = parse_tokens_to_cmd2s(tokens);
	if (!cmds)
	{
		return (NULL);
	}
	return (cmds);
}
