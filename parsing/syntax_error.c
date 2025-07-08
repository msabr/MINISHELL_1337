#include "../minishell.h"

void	error_syntax(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token && *token)
		ft_putstr_fd(token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	ft_set_status(258);
}

int	check_unclosed_quote(const char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input && input[i])
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	if (quote)
	{
		ft_set_status(258);
		return (1);
	}
	return (0);
}

const char	*token_repr(t_token *tok)
{
	if (!tok || tok->type == TOKEN_EOF)
		return ("newline");
	if (tok->type == TOKEN_PIPE)
		return ("|");
	if (tok->type == TOKEN_REDIR_IN)
		return ("<");
	if (tok->type == TOKEN_REDIR_OUT)
		return (">");
	if (tok->type == TOKEN_REDIR_APPEND)
		return (">>");
	if (tok->type == TOKEN_HEREDOC)
		return ("<<");
	if (tok->type == TOKEN_WORD || tok->type == TOKEN_VARIABLE)
		return (tok->value);
	return (tok->value ? tok->value : "");
}

int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIR_IN || t == TOKEN_REDIR_OUT
		|| t == TOKEN_REDIR_APPEND || t == TOKEN_HEREDOC);
}

int	check_syntax_errors(t_token *tokens, const char *input)
{
	t_token	*curr;

	if (!input || !*input)
	{
		ft_set_status(0);
		return (1); // Ne pas continuer
	}
	if (check_unclosed_quote(input))
		return (1);
	curr = tokens;
	if (curr && (curr->type == TOKEN_PIPE || is_redir(curr->type)))
		return (error_syntax(token_repr(curr)), 1);
	while (curr && curr->type != TOKEN_EOF)
	{
		if (curr->type == TOKEN_WORD && curr->value)
		{
			if (!ft_strcmp(curr->value, "&&") || !ft_strcmp(curr->value, "||")
				|| !ft_strcmp(curr->value, ";") || !ft_strcmp(curr->value, ";;")
				|| !ft_strcmp(curr->value, "&") || !ft_strcmp(curr->value, "(")
				|| !ft_strcmp(curr->value, ")"))
				return (error_syntax(curr->value), 1);
		}
		if (curr->type == TOKEN_PIPE && curr->next && curr->next->type == TOKEN_PIPE)
			return (error_syntax("|"), 1);
		if (curr->type == TOKEN_PIPE && (!curr->next || curr->next->type == TOKEN_EOF))
			return (error_syntax("newline"), 1);
		if (is_redir(curr->type) && curr->next && is_redir(curr->next->type))
			return (error_syntax(token_repr(curr->next)), 1);
		if (is_redir(curr->type) &&
			(!curr->next || curr->next->type == TOKEN_PIPE
				|| curr->next->type == TOKEN_EOF))
			return (error_syntax(token_repr(curr->next)), 1);
		curr = curr->next;
	}
	return (0);
}
