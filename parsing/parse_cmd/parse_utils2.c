/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 02:20:53 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 11:07:36 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

t_cmd	*new_command(void)
{
	return (ft_calloc(1, sizeof(t_cmd)));
}

int	handle_redir(t_cmd *cmd, t_token **tok)
{
	t_token	*target;
	char	*filename;
	int		quoted;

	quoted = 0;
	target = (*tok)->next;
	if (!target || !is_arg_token(target))
	{
		error_syntax("newline");
		return (0);
	}
	if (target)
		quoted = target->quoted;
	filename = merge_argument(&target);
	if (!filename)
		return (0);
	if ((*tok)->type == TOKEN_HEREDOC)
		add_redirection(&cmd->redirs, (*tok)->type, filename, quoted);
	else
		add_redirection(&cmd->redirs, (*tok)->type, filename, quoted);
	*tok = target;
	return (1);
}

int	is_only_spaces(const char *str)
{
	int	i;

	if (!str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}
