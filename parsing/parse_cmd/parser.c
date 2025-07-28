#include "../../minishell.h"

static int	handle_redir(t_cmd *cmd, t_token **tok)
{
	t_token	*target;
	char	*filename;
	int quoted = 0;

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
		add_redirection(&cmd->redirs, (*tok)->type, filename,quoted);
	else
		add_redirection(&cmd->redirs, (*tok)->type, filename,quoted);
	*tok = target;
	return (1);
}

static int is_only_spaces(const char *str) {
    if (!str) return 1;
    for (int i = 0; str[i]; i++) {
        if (!ft_isspace(str[i]))
            return 0;
    }
    return 1;
}

static void remove_empty_word_tokens(t_token **tokens)
{
    t_token *curr = *tokens, *prev = NULL, *next;
	if (curr)
        prev = curr, curr = curr->next;
    while (curr)
    {
        next = curr->next;
        if (curr->type == TOKEN_WORD && (!curr->value || curr->value[0] == '\0'))
        {
			if (prev && is_redir(prev->type))
            {
                prev = curr;
            }
            if (prev)
                prev->next = next;
            else
                *tokens = next;
        }
        else
            prev = curr;
        curr = next;
    }
}


static int	parse_tokens_loop(t_token *tok, t_cmd **cmds)
{
	t_cmd	*current = NULL;
	char	*arg;

	while (tok && tok->type != TOKEN_EOF)
	{
		if (!current)
		{
			current = new_command();
			if (!add_command(cmds, current))
				return (0);
		}

		if (is_arg_token(tok))
		{
			int was_quoted = tok->quoted;
			int was_expanded = tok->expended;
			arg = merge_argument(&tok);
			if (!arg)
				return (0);

			if (!was_quoted && arg[0] == '\0' && (current->args == NULL || current->args[0] == NULL)) {
				continue;
			}
			if (was_expanded && !was_quoted && is_only_spaces(arg)) {
				tok = tok ->next;
				continue;
			}

			if (was_quoted || arg[0] == '\0' || is_only_spaces(arg))
			{
				if (!add_argument(&current->args, arg)) {
					return (0);
				}
				arg = NULL;
			}
			if (current->args && current->args[0] && strcmp(current->args[0], "export") == 0 && was_quoted == 0) {
				if (!add_argument(&current->args, arg)) {
					return (0);
				}
				arg = NULL;
			}
			else if (current->args && current->args[0]
				&& strcmp(current->args[0], "export") == 0)
			{
				char **split_args = ft_split(arg, ' ');
				int idx = 0;
				while (split_args && split_args[idx])
				{
					if (!add_argument(&current->args, split_args[idx]))
						return (0);
					idx++;
				}
			}
			else {
				char **split_args = ft_split(arg, ' ');
				int idx = 0;
				while (split_args && split_args[idx]) {
					if (!add_argument(&current->args, split_args[idx])) {
						return (0);
					}
					idx++;
				}
			}
			continue;
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

// static int	split_and_add_args(char *arg, char ***args)
// {
// 	char	**split_args;
// 	int		idx = 0;

// 	split_args = ft_split(arg, ' ');
// 	if (!split_args)
// 		return (0);
// 	while (split_args[idx])
// 	{
// 		if (!add_argument(args, split_args[idx]))
// 			return (0);
// 		idx++;
// 	}
// 	return (1);
// }

// int	handle_argument_token(t_token **tok, t_cmd *cmd)
// {
// 	char	*arg;
// 	int		quoted = (*tok)->quoted;
// 	int		expanded = (*tok)->expended;

// 	arg = merge_argument(tok);
// 	if (!arg)
// 		return (0);
// 	if (!quoted && arg[0] == '\0' && (!cmd->args || !cmd->args[0]))
// 		return (1);
// 	if (expanded && !quoted && is_only_spaces(arg))
// 	{
// 		*tok = (*tok)->next;
// 		return (1);
// 	}
// 	if (quoted || arg[0] == '\0' || is_only_spaces(arg))
// 		return (add_argument(&cmd->args, arg));
// 	if (cmd->args && cmd->args[0] && strcmp(cmd->args[0], "export") == 0)
// 	{
// 		if (!quoted)
// 			return (add_argument(&cmd->args, arg));
// 		return (split_and_add_args(arg, &cmd->args));
// 	}
// 	return (split_and_add_args(arg, &cmd->args));
// }

// static int	handle_pipe_token(t_token **tok, t_cmd **current)
// {
// 	*current = NULL;
// 	*tok = (*tok)->next;
// 	return (1);
// }

// static int	handle_redirection_token(t_token **tok, t_cmd *current)
// {
// 	if (!handle_redir(current, tok))
// 		return (0);
// 	return (1);
// }

// static int	init_current_cmd(t_cmd **cmds, t_cmd **current)
// {
// 	*current = new_command();
// 	if (!*current)
// 		return (0);
// 	if (!add_command(cmds, *current))
// 		return (0);
// 	return (1);
// }

// int	parse_tokens_loop(t_token *tok, t_cmd **cmds)
// {
// 	t_cmd	*current = NULL;

// 	while (tok && tok->type != TOKEN_EOF)
// 	{
// 		if (!current && !init_current_cmd(cmds, &current))
// 			return (0);
// 		if (is_arg_token(tok) && !handle_argument_token(&tok, current))
// 			return (0);
// 		else if (is_redir(tok->type) && !handle_redirection_token(&tok, current))
// 			return (0);
// 		else if (tok->type == TOKEN_PIPE && !handle_pipe_token(&tok, &current))
// 			return (0);
// 		else
// 			tok = tok->next;
// 	}
// 	return (1);
// }

t_cmd	*parse_tokens_to_cmd2s(t_token *tokens)
{
	t_cmd	*cmds = NULL;
	remove_empty_word_tokens(&tokens);

	if (!tokens || (tokens->type == TOKEN_EOF && !tokens->next))
		return (NULL);
	if (!parse_tokens_loop(tokens, &cmds))
		return (NULL);
	return (cmds);
}


