#include "../../minishell.h"

static int	handle_redir(t_cmd *cmd, t_token **tok)
{
	t_token	*target;
	char	*filename;

	target = (*tok)->next;
	if (!target || !is_arg_token(target))
	{
		error_syntax("newline");
		return (0);
	}
	filename = merge_argument(&target);
	if (!filename)
		return (0);
	if ((*tok)->type == TOKEN_HEREDOC)
		add_redirection(&cmd->redirs, (*tok)->type, NULL, filename);
	else
		add_redirection(&cmd->redirs, (*tok)->type, filename, NULL);
	*tok = target;
	return (1);
}

static int	parse_tokens_loop(t_token *tok, t_cmd **cmds)
{
	t_cmd	*current = NULL;
	char	*arg;

	while (tok && tok->type != TOKEN_EOF)
	{
		// Ignore les tokens vides (déjà supprimés mais sécurité)
		// if (is_arg_token(tok) && (!tok->value || !tok->value[0]))
		// {
		// 	tok = tok->next;
		// 	continue;
		// }
		if (!current)
		{
			current = new_command();
			if (!add_command(cmds, current))
				return (0);
		}
		if (is_arg_token(tok))
		{
			arg = merge_argument(&tok);
			if (!arg || !add_argument(&current->args, arg))
				return (0);
			continue ;
		}
		if (is_redir(tok->type))
		{
			if (!handle_redir(current, &tok))
				return (0);
			continue ;
		}
		if (tok->type == TOKEN_PIPE)
		{
			current = NULL;
			tok = tok->next;
			continue;
		}
		tok = tok->next;
	}
	return (1);
}

/*
** Point d'entrée : parse une liste de token (sans tokens vides) en commandes
*/
t_cmd	*parse_tokens_to_cmd2s(t_token *tokens)
{
	t_cmd	*cmds = NULL;

	// remove_empty_token_head(&tokens);
	if (!tokens || (tokens->type == TOKEN_EOF && !tokens->next))
		return (NULL);
	if (!parse_tokens_loop(tokens, &cmds))
	{
		free_cmd_list(cmds);
		return (NULL);
	}
	return (cmds);
}