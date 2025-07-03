#include "../minishell.h"
#include <unistd.h>

/* Affiche le message d'erreur bash-like et set status */
void	syntax_error(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	if (token && token[0])
		ft_putstr_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_set_status(2);
}

int	check_quotes(const char *input)
{
	int	sq = 0, dq = 0;
	size_t i = 0;
	while (input && input[i])
	{
		if (input[i] == '\'' && dq % 2 == 0)
			sq++;
		else if (input[i] == '"' && sq % 2 == 0)
			dq++;
		i++;
	}
	return (sq % 2 == 0 && dq % 2 == 0);
}

static int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIR_IN || t == TOKEN_REDIR_OUT
		|| t == TOKEN_REDIR_APPEND || t == TOKEN_HEREDOC);
}

/* Nom lisible pour le token (pour l'affichage de l'erreur) */
static const char *token_repr(t_token *tok)
{
	if (!tok || tok->type == TOKEN_EOF)
		return "newline";
	if (tok->type == TOKEN_PIPE)
		return "|";
	if (tok->type == TOKEN_REDIR_IN)
		return "<";
	if (tok->type == TOKEN_REDIR_OUT)
		return ">";
	if (tok->type == TOKEN_REDIR_APPEND)
		return ">>";
	if (tok->type == TOKEN_HEREDOC)
		return "<<";
	if (tok->type == TOKEN_WORD || tok->type == TOKEN_VARIABLE)
		return tok->value;
	if (tok->type == TOKEN_SQUOTE)
		return "'";
	if (tok->type == TOKEN_DQUOTE)
		return "\"";
	return tok->value ? tok->value : "";
}

/* Handler principal */
int	check_syntax_errors(t_token *tokens, const char *input)
{
	t_token	*curr = tokens;
	if (!check_quotes(input))
		return (syntax_error("newline"), 1);
	
	// Check for pipe at the beginning of input
	if (curr && curr->type == TOKEN_PIPE)
		return (syntax_error("|"), 1);
		
	if (is_redir(curr->type) && curr->next && is_redir(curr->next->type))
    	return (syntax_error(token_repr(curr->next)), 1);
	while (curr && curr->type != TOKEN_EOF)
	{
		if (curr->type == TOKEN_PIPE && curr->next && curr->next->type == TOKEN_PIPE)
			return (syntax_error("||"), 1);
		if (curr->type == TOKEN_WORD && curr->value && curr->value[0] == '&'
			&& curr->value[1] == '&')
			return (syntax_error("&&"), 1);

		// Check for semicolon patterns ;;
		if (curr->type == TOKEN_WORD && curr->value && ft_strstr(curr->value, ";;"))
			return (syntax_error(";;"), 1);
			
		// Check for parentheses () which are not supported
		if (curr->type == TOKEN_WORD && curr->value && 
			(ft_strchr(curr->value, '(') || ft_strchr(curr->value, ')')))
			return (syntax_error(ft_strchr(curr->value, '(') ? "(" : ")"), 1);

		if (is_redir(curr->type) && curr->next && is_redir(curr->next->type))
			return (syntax_error(token_repr(curr->next)), 1);

		if (is_redir(curr->type))
		{
			if (!curr->next || curr->next->type == TOKEN_PIPE ||
				is_redir(curr->next->type) || curr->next->type == TOKEN_EOF)
				return (syntax_error(token_repr(curr->next)), 1);
		}
		if (curr->type == TOKEN_PIPE && curr->next && is_redir(curr->next->type))
			return (syntax_error(token_repr(curr->next)), 1);

		// PIPE à la fin
		if (curr->type == TOKEN_PIPE && (!curr->next || curr->next->type == TOKEN_EOF))
			return (syntax_error("newline"), 1);

		curr = curr->next;
	}
	return (0);
}
