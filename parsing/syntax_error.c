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
	if (tok->value)
		return (tok->value);
	return ("");
}

static int	is_valid_word(char *value)
{
	if (!value)
		return (1);
	while (*value)
	{
		if (*value == '&' || *value == '(' || *value == ')' || *value == ';')
		{
			ft_putstr_fd("minishell: syntax error near unexpected token  `", 2);
			ft_putchar_fd(*value, 2);
			ft_putstr_fd("'\n", 2);
			ft_set_status(258);
			return (0);
		}
		value++;
	}
	return (1);
}

// ---> debut de fonction diviser 
static int	check_input_and_quotes(const char *input)
{
	if (!input || !*input)
	{
		ft_set_status(0);
		return (0);
	}
	if (check_unclosed_quote(input))
		return (1);
	return (-1);
}

static int	check_pipe_or_redir_start(t_token *curr)
{
	if (curr && curr->type == TOKEN_PIPE)
		return (error_syntax(token_repr(curr)), 1);
	if (curr && is_redir(curr->type) && curr->type != TOKEN_HEREDOC)
	{
		if (!curr->next || !is_arg_token(curr->next))
			return (error_syntax(token_repr(curr)), 1);
	}
	return (0);
}

static int	check_token_errors(t_token *curr)
{
	while (curr && curr->type != TOKEN_EOF)
	{
		if (curr->type == TOKEN_WORD && curr->value)
		{
			if (!curr->expended && !is_valid_word(curr->value))
				return (1);
		}
		if (curr->type == TOKEN_PIPE && curr->next
			&& curr->next->type == TOKEN_PIPE)
			return (error_syntax("|"), 1);
		if (curr->type == TOKEN_PIPE
			&& (!curr->next || curr->next->type == TOKEN_EOF))
			return (error_syntax("newline"), 1);
		if (is_redir(curr->type) && curr->next
			&& is_redir(curr->next->type))
			return (error_syntax(token_repr(curr->next)), 1);
		if (curr->type == TOKEN_HEREDOC)
		{
			if (!curr->next || curr->next->type == TOKEN_EOF
				|| !curr->next->value || curr->next->value[0] == '\0')
				return (error_syntax("newline"), 1);
		}
		curr = curr->next;
	}
	return (0);
}

static int	is_only_redir_and_args(t_token *tokens)
{
	t_token	*curr;
	int		only_redir;

	only_redir = 1;
	curr = tokens;
	while (curr && curr->type != TOKEN_EOF)
	{
		if (!is_redir(curr->type) && !is_arg_token(curr))
			only_redir = 0;
		curr = curr->next;
	}
	return (only_redir);
}

int	check_syntax_errors(t_token *tokens, const char *input)
{
	t_token	*curr;
	int		ret;

	ret = check_input_and_quotes(input);
	if (ret != -1)
		return (ret);
	curr = tokens;
	ret = check_pipe_or_redir_start(curr);
	if (ret)
		return (1);
	ret = check_token_errors(curr);
	if (ret)
		return (1);
	if (is_only_redir_and_args(tokens))
		return (0);
	return (0);
}
