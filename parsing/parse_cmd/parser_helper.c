/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 23:55:48 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 12:30:24 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

static t_heredoc	*create_heredoc(char *filename, int quoted_name)
{
	t_heredoc	*heredoc;

	heredoc = ft_malloc(sizeof(t_heredoc));
	if (!heredoc)
		return (NULL);
	heredoc->delimiter = filename;
	heredoc->fd_read = -1;
	heredoc->fd_write = -1;
	heredoc->flag = quoted_name;
	heredoc->env = NULL;
	return (heredoc);
}

t_redir	*new_redir(t_token_type type, char *filename, int quoted_name)
{
	t_redir	*new;

	new = ft_malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->filename = filename;
	new->fd_in = -1;
	new->fd_out = -1;
	new->next = NULL;
	if (type == TOKEN_HEREDOC)
	{
		new->heredoc = create_heredoc(filename, quoted_name);
		if (!new->heredoc)
			return (NULL);
	}
	else
		new->heredoc = NULL;
	return (new);
}

int	add_redirection(t_redir **redir, t_token_type type, char *fname, int q_name)
{
	t_redir	*new;
	t_redir	*tmp;

	new = new_redir(type, fname, q_name);
	if (!new)
		return (0);
	if (!*redir)
		*redir = new;
	else
	{
		tmp = *redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

int	add_command(t_cmd **cmds, t_cmd *new)
{
	t_cmd	*tmp;

	if (!new)
		return (0);
	if (!*cmds)
		*cmds = new;
	else
	{
		tmp = *cmds;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

char	*merge_argument(t_token **ptok)
{
	size_t	len;
	char	*arg;
	t_token	*tok;

	len = arg_total_len(*ptok);
	arg = ft_malloc(len + 1);
	tok = *ptok;
	if (!arg)
		return (NULL);
	arg[0] = 0;
	while (is_arg_token(tok))
	{
		if ((tok->type == TOKEN_SQUOTE || tok->type == TOKEN_DQUOTE)
			&& tok->value[0] == 0 && tok->space_after)
			return (*ptok = tok->next, ft_strdup(""));
		ft_strlcat(arg, tok->value, len + 1);
		if (tok->space_after)
		{
			*ptok = tok->next;
			return (arg);
		}
		tok = tok->next;
	}
	return (*ptok = tok, arg);
}
