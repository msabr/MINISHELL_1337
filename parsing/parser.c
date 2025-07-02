/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 06:17:21 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/01 17:16:20 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


/* ---------- Utils for args ---------- */
static int	ft_tablen(char **tab)
{
	int i = 0;
	if (!tab)
		return 0;
	while (tab[i])
		i++;
	return i;
}

static char	**ft_tabadd(char **tab, const char *word)
{
	int		len = ft_tablen(tab);
	char	**res = malloc(sizeof(char *) * (len + 2));
	int		i = 0;

	if (!res)
		return NULL;
	while (i < len)
	{
		res[i] = tab[i];
		i++;
	}
	res[i++] = ft_strdup(word);
	res[i] = NULL;
	free(tab);
	return res;
}

/* ---------- Redirection List ---------- */
static t_redir	*redir_new(t_token_type type, const char *filename)
{
	t_redir *r = malloc(sizeof(t_redir));
	if (!r)
		return NULL;
	r->type = type;
	r->filename = ft_strdup(filename);
	r->next = NULL;
	return r;
}

static void	redir_addback(t_redir **lst, t_redir *new)
{
	t_redir *cur = *lst;
	if (!*lst)
	{
		*lst = new;
		return;
	}
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

/* ---------- Heredoc List ---------- */
static t_heredoc *heredoc_new(const char *delimiter)
{
	t_heredoc *h = malloc(sizeof(t_heredoc));
	if (!h)
		return NULL;
	h->delimiter = ft_strdup(delimiter);
	h->content = NULL;
	h->next = NULL;
	return h;
}

static void	heredoc_addback(t_heredoc **lst, t_heredoc *new)
{
	t_heredoc *cur = *lst;
	if (!*lst)
	{
		*lst = new;
		return;
	}
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

/* ---------- Command List ---------- */
static t_cmd	*cmd_new(void)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return NULL;
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->heredocs = NULL;
	cmd->next = NULL;
	return cmd;
}

static void	cmd_addback(t_cmd **lst, t_cmd *new)
{
	t_cmd *cur = *lst;
	if (!*lst)
	{
		*lst = new;
		return;
	}
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

/* ---------- Is redirection type ---------- */
static bool	is_redir_type(t_token_type type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND
		|| type == TOKEN_HEREDOC);
}

/* ---------- Parser main function ---------- */
t_cmd	*parse_tokens_to_cmds(t_token *tokens)
{
	t_cmd	*cmds = NULL;
	t_cmd	*current = NULL;
	t_token	*tok = tokens;

	while (tok && tok->type != TOKEN_EOF)
	{
		// Create new command at the start or after a PIPE
		if (!current)
		{
			current = cmd_new();
			if (!current)
				return NULL;
			cmd_addback(&cmds, current);
		}
		// Arguments (WORD or VARIABLE)
		if (tok->type == TOKEN_WORD || tok->type == TOKEN_VARIABLE)
		{
			current->args = ft_tabadd(current->args, tok->value);
		}
		// Redirections
		else if (is_redir_type(tok->type))
		{
			if (tok->next && (tok->next->type == TOKEN_WORD || tok->next->type == TOKEN_VARIABLE))
			{
				if (tok->type == TOKEN_HEREDOC)
				{
					heredoc_addback(&(current->heredocs), heredoc_new(tok->next->value));
					redir_addback(&(current->redirs), redir_new(tok->type, tok->next->value));
				}
				else
				{
					redir_addback(&(current->redirs), redir_new(tok->type, tok->next->value));
				}
				tok = tok->next; // skip target/filename/delimiter
			}
			else
			{
				// Syntax error: redirection sans cible
				// (Tu peux ajouter un gestionnaire d'erreur ici)
				return NULL;
			}
		}
		// Pipe
		else if (tok->type == TOKEN_PIPE)
		{
			current = NULL;
		}
		tok = tok->next;
	}
	return cmds;
}