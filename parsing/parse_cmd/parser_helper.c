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
		if ((tok->type == TOKEN_SQUOTE || tok->type == TOKEN_DQUOTE)
			&& tok->value[0] == 0 && tok->space_after)
		{
			*ptok = tok->next;
			return (ft_strdup(""));
		}
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
	*args = new_args;
	return (1);
}



t_redir *new_redir(t_token_type type, char *filename,int quoted_name)
{
    t_redir *new = ft_malloc(sizeof(t_redir));
    if (!new)
        return NULL;

    new->type = type;
    new->filename = filename;
    new->fd_in = -1;
    new->fd_out = -1;
    new->next = NULL;

    if (type == TOKEN_HEREDOC)
    {
        new->heredoc = ft_malloc(sizeof(t_heredoc));
        if (!new->heredoc)
        {
            free(new);
            return NULL;
        }
        new->heredoc->delimiter = filename;
        new->heredoc->fd_read = -1;
        new->heredoc->fd_write = -1;
        new->heredoc->heredoc_num = 0; // À gérer si plusieurs heredocs
        new->heredoc->index = 0;       // À gérer si besoin
        new->heredoc->flag = quoted_name;        // À renseigner selon si le heredoc est quoté
        new->heredoc->env = NULL;      // À remplir si besoin d'environnement
    }
    else
    {
        new->heredoc = NULL;
    }
    return new;
}

int	add_redirection(t_redir **redir, t_token_type type, char *filename,int quoted_name)
{
	t_redir	*new = new_redir(type, filename,quoted_name);
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

// Supprime tous les tokens vides (value == NULL ou value[0] == 0) de la liste chainée
void	remove_empty_token(t_token **tokens)
{
	t_token	*curr;
	t_token	*prev;

	prev = NULL;
	curr = *tokens;
	while (curr)
	{
		if (!curr->value || curr->value[0] == '\0')
		{
			// to_free = curr;
			if (prev)
				prev->next = curr->next;
			else
				*tokens = curr->next;
			curr = curr->next;

		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}
