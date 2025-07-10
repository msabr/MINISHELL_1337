#include "../../minishell.h"

int	is_arg_token(t_token *tok)
{
	return (tok && (tok->type == TOKEN_WORD || tok->type == TOKEN_VARIABLE
		|| tok->type == TOKEN_DQUOTE || tok->type == TOKEN_SQUOTE));
}

// Helper: vérifie si un type est une redirection
int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIR_IN || t == TOKEN_REDIR_OUT
		|| t == TOKEN_REDIR_APPEND || t == TOKEN_HEREDOC);
}

// Helper: fusionne plusieurs tokens d'arguments en une seule string
static size_t	arg_total_len(t_token *tok)
{
	size_t	len = 0;
	while (is_arg_token(tok) && !tok->space_after)
	{
		len += ft_strlen(tok->value);
		tok = tok->next;
	}
	if (is_arg_token(tok))
		len += ft_strlen(tok->value);
	return (len);
}

char	*merge_argument(t_token **ptok)
{
	size_t	len = arg_total_len(*ptok);
	char	*arg = ft_malloc(len + 1);
	t_token	*tok = *ptok;

	if (!arg)
		return (NULL);
	arg[0] = 0;
	while (is_arg_token(tok))
	{
		ft_strlcat(arg, tok->value, len + 1);
		if (tok->space_after)
		{
			*ptok = tok->next;
			return (arg);
		}
		tok = tok->next;
	}
	*ptok = tok;
	return (arg);
}

static int	count_args(char **args)
{
	int	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}

int	add_argument(char ***args, char *new_arg)
{
	int	argc = count_args(*args);
	char	**new_args = ft_malloc(sizeof(char *) * (argc + 2));
	int	i = 0;

	if (!new_args)
		return (0);
	while (i < argc)
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[argc] = new_arg;
	new_args[argc + 1] = NULL;
	free(*args);
	*args = new_args;
	return (1);
}

t_redir	*new_redir(t_token_type type, char *filename, char *delimiter_heredoc)
{
	t_redir	*new = ft_malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->filename = filename;
	new->fd_in = -1;
	new->fd_out = -1;
	new->exit_status = 0;
	new->delimiter_heredoc = delimiter_heredoc;
	new->heredoc_content = NULL;
	new->next = NULL;
	return (new);
}

int	add_redirection(t_redir **redir, t_token_type type, char *filename, char *delimiter_heredoc)
{
	t_redir	*new = new_redir(type, filename, delimiter_heredoc);
	t_redir	*tmp;

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

t_cmd	*new_command(void)
{
	return (ft_calloc(1, sizeof(t_cmd)));
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

void	free_cmd_list(t_cmd *cmds)
{
	t_cmd	*tmp;
	t_redir	*rtmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		i = 0;
		if (cmds->args)
		{
			while (cmds->args[i])
				free(cmds->args[i++]);
			free(cmds->args);
		}
		while (cmds->redirs)
		{
			rtmp = cmds->redirs->next;
			free(cmds->redirs->filename);
			free(cmds->redirs->delimiter_heredoc);
			free(cmds->redirs);
			cmds->redirs = rtmp;
		}
		free(cmds);
		cmds = tmp;
	}
}

// Supprime tous les tokens vides (value == NULL ou value[0] == 0) de la liste chainée
void	remove_empty_token(t_token **tokens)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*to_free;

	prev = NULL;
	curr = *tokens;
	while (curr)
	{
		if (!curr->value || curr->value[0] == '\0')
		{
			to_free = curr;
			if (prev)
				prev->next = curr->next;
			else
				*tokens = curr->next;
			curr = curr->next;
			free(to_free->value);
			free(to_free);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

void	remove_empty_token_head(t_token **tokens)
{
	t_token *tmp;
	while (*tokens && (!(*tokens)->value || (*tokens)->value[0] == '\0'))
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->value);
		free(tmp);
	}
}