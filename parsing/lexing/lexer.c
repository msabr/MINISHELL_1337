#include "../../minishell.h"

static int	is_quote_closed(const char *input, size_t i, char quote)
{
	while (input[i])
	{
		if (input[i] == quote)
			return (1);
		if (quote == '"' && input[i] == '\\' && input[i + 1])
			i += 2;
		else
			i++;
	}
	return (0);
}

static void	handle_word(const char *input, size_t *i, t_token **head)
{
	size_t	len;
	char	*val;
	bool	space;

	len = 0;
	while (input[*i + len] && !is_operator(input[*i + len])
		&& input[*i + len] != '\'' && input[*i + len] != '"'
		&& !is_whitespace(input[*i + len]))
	{
		if (input[*i + len] == '\\' && input[*i + len + 1])
			len += 2;
		else
			len++;
	}
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	if (val && val[0] != '\0')
		add_token(head, val, TOKEN_WORD, space,0);
	*i += len;
}


static void	handle_quote_token(const char *input, size_t *i, char quote, t_token **head)
{
	size_t			start;
	char			*val;
	bool			space;
	t_token_type	type;

	start = *i;
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '"' && input[*i] == '\\' && input[*i + 1])
			*i += 2;
		else
			(*i)++;
	}
	val = ft_strndup(input + start, *i - start);
	if (input[*i + 1] == '\0' || is_whitespace(input[*i + 1]))
		space = 1;
	else
		space = 0;
	if (quote == '\'')
		type = TOKEN_SQUOTE;
	else
		type = TOKEN_DQUOTE;
	add_token_quoted(head, val, type, space, 1);
	if (input[*i])
		(*i)++;
}

static void	handle_quote(const char *input, size_t *i, t_token **head)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	if (!is_quote_closed(input, *i, quote))
	{
		error_syntax("unclosed quote");
		return ;
	}
	handle_quote_token(input, i, quote, head);
}
static size_t	variable_length(const char *input, size_t i)
{
	size_t	len;

	len = 1;
	if (input[i + 1] == '?')
		return (2);
	while (input[i + len])
	{
		if ((input[i + len] >= 'a' && input[i + len] <= 'z')
			|| (input[i + len] >= 'A' && input[i + len] <= 'Z')
			|| (input[i + len] >= '0' && input[i + len] <= '9')
			|| input[i + len] == '_')
			len++;
		else
			break ;
	}
	return (len);
}

static void	handle_variable(const char *input, size_t *i, t_token **head)
{
	size_t	len;
	char	*val;
	bool	space;

	len = variable_length(input, *i);
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	add_token(head, val, TOKEN_VARIABLE, space,1);
	*i += len;
}

static void	handle_operator(const char *input, size_t *i, t_token **head)
{
	size_t	len;
	char	*val;
	bool	space;
	t_token_type	type;

	type = get_operator_type(input + *i);
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		len = 2;
	else
		len = 1;
	val = ft_strndup(input + *i, len);
	if (input[*i + len] == '\0' || is_whitespace(input[*i + len]))
		space = 1;
	else
		space = 0;
	add_token(head, val, type, space,0);
	*i += len;
}

t_token	*lexer(const char *input)
{
	t_token	*head;
	size_t	i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
		{
			while (input[i] && is_whitespace(input[i]))
				i++;
		}
		else if (is_operator(input[i]))
			handle_operator(input, &i, &head);
		else if (input[i] == '\'' || input[i] == '"')
			handle_quote(input, &i, &head);
		else if (input[i] == '$')
			handle_variable(input, &i, &head);
		else
			handle_word(input, &i, &head);
	}
	add_token(&head, "", TOKEN_EOF, 0,0);
	return (head);
}
