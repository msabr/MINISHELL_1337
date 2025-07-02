#include "../minishell.h"

int	is_arg_token(t_token *tok)
{
	if (!tok)
		return (0);
	return (tok->type == TOKEN_WORD || tok->type == TOKEN_VARIABLE
		|| tok->type == TOKEN_DQUOTE || tok->type == TOKEN_SQUOTE);
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

char	*merge_argument(t_token **ptok)
{
	size_t	len;
	char	*arg;
	t_token	*tok;

	len = arg_total_len(*ptok);
	arg = malloc(len + 1);
	if (!arg)
		return (NULL);
	arg[0] = 0;
	tok = *ptok;
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

void	add_argument(char ***args, char *new_arg)
{
	int	argc;
	char	**new_args;
	int	i;

	argc = count_args(*args);
	new_args = malloc(sizeof(char *) * (argc + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < argc)
	{
		new_args[i] = (*args)[i];
		i++;
	}
	new_args[argc] = new_arg;
	new_args[argc + 1] = NULL;
	free(*args);
	*args = new_args;
}

t_redir	*new_redir(t_token_type type, char *filename)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->type = type;
	new->filename = filename;
	new->next = NULL;
	return (new);
}

void	add_redirection(t_redir **redir, t_token_type type, char *filename)
{
	t_redir	*new;
	t_redir	*tmp;

	new = new_redir(type, filename);
	if (!new)
		return ;
	if (!*redir)
		*redir = new;
	else
	{
		tmp = *redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_heredoc	*new_heredoc(char *delim)
{
	t_heredoc	*hd;

	hd = malloc(sizeof(t_heredoc));
	if (!hd)
		return (NULL);
	hd->delimiter = delim;
	hd->content = NULL;
	hd->next = NULL;
	return (hd);
}

void	add_heredoc(t_heredoc **hd, char *delim)
{
	t_heredoc	*new;
	t_heredoc	*tmp;

	new = new_heredoc(delim);
	if (!new)
		return ;
	if (!*hd)
		*hd = new;
	else
	{
		tmp = *hd;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_cmd	*new_command(void)
{
	t_cmd	*new;

	new = ft_calloc(1, sizeof(t_cmd));
	return (new);
}

void	add_command(t_cmd **cmds, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*cmds)
		*cmds = new;
	else
	{
		tmp = *cmds;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void	handle_redir(t_cmd *cmd, t_token **tok)
{
	t_token	*target;
	char	*filename;

	target = (*tok)->next;
	if (!target || !is_arg_token(target))
	{
		write(2, "minishell: syntax error: missing redir target\n", 47);
		return ;
	}
	filename = merge_argument(&target);
	if ((*tok)->type == TOKEN_HEREDOC)
		add_heredoc(&cmd->heredocs, filename);
	else
		add_redirection(&cmd->redirs, (*tok)->type, filename);
	*tok = target;
}

t_cmd	*parse_tokens_to_cmds(t_token *tok)
{
	t_cmd	*cmds;
	t_cmd	*current;
	char	*arg;

	cmds = NULL;
	current = NULL;
	while (tok && tok->type != TOKEN_EOF)
	{
		if (!current)
		{
			current = new_command();
			add_command(&cmds, current);
		}
		if (is_arg_token(tok))
		(1) &&
		(arg = merge_argument(&tok),add_argument(&current->args, arg),continue )
		if (tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
			|| tok->type == TOKEN_REDIR_APPEND || tok->type == TOKEN_HEREDOC)
			handle_redir(current, &tok);
		else if (tok->type == TOKEN_PIPE)
			current = NULL;
		tok = tok->next;
	}
	return (cmds);
}
