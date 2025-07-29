/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:24:43 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 14:42:37 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

int	process_split_args(t_cmd *current, char *arg)
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

int	process_export_logic(t_cmd *current, char *arg, int was_quoted)
{
	if (current->args && current->args[0]
		&& ft_strcmp(current->args[0], "export") == 0 && was_quoted == 0)
	{
		if (!add_argument(&current->args, arg))
			return (0);
		return (1);
	}
	else if (current->args && current->args[0]
		&& ft_strcmp(current->args[0], "export") == 0)
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
