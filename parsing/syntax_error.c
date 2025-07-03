#include "../minishell.h"

/* Affiche le message d'erreur bash-like et set status */
void	syntax_error(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	if (token && token[0])
		ft_putstr_fd(token, STDERR_FILENO);
	else
		ft_putstr_fd("newline", STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	ft_set_status(2); // Changé de 258 à 2 pour correspondre à bash
}

/* Vérifie si les quotes sont bien fermées */
int	check_quotes(const char *input)
{
	int		sq;
	int		dq;
	size_t	i;

	sq = 0;
	dq = 0;
	i = 0;
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

/* Vérifie si c'est un token de redirection */
static int	is_redir(t_token_type t)
{
	return (t == TOKEN_REDIR_IN || t == TOKEN_REDIR_OUT
		|| t == TOKEN_REDIR_APPEND || t == TOKEN_HEREDOC);
}

/* Vérifie si le token contient des opérateurs non supportés */
static int	contains_unsupported_ops(const char *value)
{
	if (!value)
		return (0);
	
	// Vérifier && au début
	if (value[0] == '&' && value[1] == '&')
		return (1);
	
	// Vérifier ;; 
	if (value[0] == ';' && value[1] == ';')
		return (1);
	
	// Vérifier les parenthèses
	if (value[0] == '(' || value[0] == ')')
		return (1);
	
	return (0);
}

/* Obtient le token d'erreur approprié pour les opérateurs non supportés */
static const char	*get_unsupported_token(const char *value)
{
	if (!value)
		return ("");
	
	if (value[0] == '&' && value[1] == '&')
		return ("&&");
	if (value[0] == ';' && value[1] == ';')
		return (";;");
	if (value[0] == ')')
		return (")");
	if (value[0] == '(')
		return ("(");
	
	return (value);
}

/* Nom lisible pour le token (pour l'affichage de l'erreur) */
static const char	*token_repr(t_token *tok)
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
	if (tok->type == TOKEN_SQUOTE)
		return ("'");
	if (tok->type == TOKEN_DQUOTE)
		return ("\"");
	return (tok->value ? tok->value : "");
}

/* Vérifie les redirections consécutives spécifiques */
static int	check_consecutive_redirs(t_token *curr)
{
	t_token	*next;

	if (!is_redir(curr->type))
		return (0);
	
	next = curr->next;
	if (!next || !is_redir(next->type))
		return (0);
	
	// Cas spéciaux pour des séquences spécifiques
	if (curr->type == TOKEN_REDIR_OUT && next->type == TOKEN_REDIR_OUT)
		return (syntax_error(">>"), 1);
	if (curr->type == TOKEN_REDIR_APPEND && next->type == TOKEN_REDIR_OUT)
		return (syntax_error(">>"), 1);
	if (curr->type == TOKEN_HEREDOC && next->type == TOKEN_HEREDOC)
		return (syntax_error("<<"), 1);
	
	return (syntax_error(token_repr(next)), 1);
}

/* Vérifie si l'input est vide ou contient seulement des espaces/tabs */
static int	is_empty_or_whitespace(const char *input)
{
	size_t	i;

	if (!input)
		return (1);
	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
			return (0);
		i++;
	}
	return (1);
}

/* Handler principal des erreurs de syntaxe */
int	check_syntax_errors(t_token *tokens, const char *input)
{
	t_token	*curr;

	// Vérifier les quotes non fermées
	if (!check_quotes(input))
		return (syntax_error("newline"), 1);
	
	// Gérer l'input vide ou seulement des espaces/tabs
	if (is_empty_or_whitespace(input))
		return (0);
	
	curr = tokens;
	if (!curr || curr->type == TOKEN_EOF)
		return (0);
	
	// Vérifier pipe au début
	if (curr->type == TOKEN_PIPE)
		return (syntax_error("|"), 1);
	
	while (curr && curr->type != TOKEN_EOF)
	{
		// Vérifier les opérateurs non supportés dans les tokens WORD
		if (curr->type == TOKEN_WORD && contains_unsupported_ops(curr->value))
			return (syntax_error(get_unsupported_token(curr->value)), 1);
		
		// Vérifier les pipes consécutifs
		if (curr->type == TOKEN_PIPE && curr->next && curr->next->type == TOKEN_PIPE)
			return (syntax_error("||"), 1);
		
		// Vérifier les redirections consécutives
		if (check_consecutive_redirs(curr))
			return (1);
		
		// Vérifier redirection sans argument
		if (is_redir(curr->type))
		{
			if (!curr->next || curr->next->type == TOKEN_PIPE ||
				is_redir(curr->next->type) || curr->next->type == TOKEN_EOF)
				return (syntax_error(token_repr(curr->next)), 1);
		}
		
		// Vérifier pipe suivi d'une redirection
		if (curr->type == TOKEN_PIPE && curr->next && is_redir(curr->next->type))
			return (syntax_error(token_repr(curr->next)), 1);
		
		// Vérifier pipe à la fin
		if (curr->type == TOKEN_PIPE && (!curr->next || curr->next->type == TOKEN_EOF))
			return (syntax_error("newline"), 1);
		
		curr = curr->next;
	}
	return (0);
}