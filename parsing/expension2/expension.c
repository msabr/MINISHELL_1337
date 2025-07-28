# include "../../minishell.h"

int	was_originally_quoted(t_token *token)
{
	if (!token)
		return (0);
	return (token->quoted && (token->type == TOKEN_SQUOTE || token->type == TOKEN_DQUOTE));
}

int	is_in_heredoc(t_token *token)
{
	t_token	*prev = token->prev;
	while (prev && prev->type == TOKEN_WORD && prev->space_after == false)
		prev = prev->prev;
	while (prev && prev->type == TOKEN_WORD && prev->space_after == true)
		prev = prev->prev;
	if (prev && prev->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}


static void	expansion_handle_heredoc(t_token *curr)
{
	// Utilise le champ quoted pour savoir si le token était quoté à l'origine
	if (was_originally_quoted(curr))
	{
		if (curr->type == TOKEN_DQUOTE)
			curr->value = remove_dquotes(curr->value);
		else if (curr->type == TOKEN_SQUOTE)
			curr->value = remove_squotes(curr->value);
	}
	curr->type = TOKEN_WORD;
}

static void	expansion_handle_variable(t_token *curr, t_env *env)
{
	int		i = 0;
	int		dollar_count = 0;
	char	*expanded = NULL;

	while (curr->value[i] == '$')
	{
		dollar_count++;
		i++;
	}
	const char *var_name = curr->value + i;
	if (var_name[0])
	{
		if (dollar_count % 2 == 1)
		{
			char *val = get_env_value(&env, var_name);
			if (val)
			{
				expanded = ft_strdup(val);
				curr->expended = 0;

			}
			else
			{	
				expanded = ft_strdup("");
				curr->expended = 1;
			}

		}
		else
		{
			expanded = ft_strdup(var_name);
			curr->expended = 0;
		}
	}
	else
	{
		expanded = ft_strdup(curr->value);
		curr->expended = 0;
	}
	curr->value = expanded;
	curr->type = TOKEN_WORD;
}

static void	expansion_handle_dquote(t_token *curr, t_env *env)
{
	expand_dquote_token(curr, env);
}

static void	expansion_handle_squote(t_token *curr)
{
	expand_squote_token(curr);
}

/*
** Cas 5 : handle un token WORD générique (expansion dans les mots collés)
*/
static void	expansion_handle_word(t_token *curr, t_env *env)
{
	char *expanded = expand_variables_in_word(curr->value, env);
	curr->value = expanded;
}

char	*expand_variables_in_string(const char *str, t_env *env)
{
	size_t	i = 0, j = 0, len = 0;
	size_t	tmp_len = 0;
	char	*result;

	// 1. CALCUL DE LA TAILLE RÉELLE DU RÉSULTAT
	while (str[len])
	{
		if (str[len] == '$')
		{
			size_t varlen = 0;
			if (str[len + 1] == '?')
			{
				char *exit_code = ft_itoa(*ft_get_status());
				tmp_len += ft_strlen(exit_code);
				// free(exit_code); // Si ft_itoa alloue
				len += 2;
				continue;
			}
			else if (ft_isalpha(str[len + 1]) || str[len + 1] == '_')
			{
				varlen = 1;
				while (str[len + varlen] && (ft_isalnum(str[len + varlen]) || str[len + varlen] == '_'))
					varlen++;
				char *key = ft_malloc(varlen);
				if (!key) return NULL;
				for (size_t k = 0; k < varlen - 1; ++k)
					key[k] = str[len + 1 + k];
				key[varlen - 1] = '\0';
				char *val = get_env_value(&env, key);
				tmp_len += val ? ft_strlen(val) : 0;
				// free(key);
				len += varlen;
				continue;
			}
            else if (ft_isdigit(str[len + 1]))
            {
                len += 2;
                continue;
            }
			else
			{
				tmp_len += 1; // pour le '$'
				if (str[len + 1])
					tmp_len += 1;
				len += (str[len + 1]) ? 2 : 1;
				continue;
			}
		}
		else
		{
			tmp_len += 1;
			len++;
		}
	}
	// 2. ALLOCATION DE LA BONNE TAILLE
	result = ft_malloc(tmp_len + 1);
	if (!result)
		return (NULL);

	// 3. COPIE EFFECTIVE AVEC EXPANSION
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			if (str[i] == '?')
			{
				char *exit_code = ft_itoa(*ft_get_status());
				size_t k = 0;
				while (exit_code && exit_code[k])
					result[j++] = exit_code[k++];
				// free(exit_code);
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				size_t varlen = 0;
				while (str[i + varlen] && (ft_isalnum(str[i + varlen]) || str[i + varlen] == '_'))
					varlen++;
				char *key = ft_malloc(varlen + 1);
				if (!key) return NULL;
				size_t k = 0;
				while (k < varlen)
				{
					key[k] = str[i + k];
					k++;
				}
				key[varlen] = '\0';
				char *val = get_env_value(&env, key);
				if (val)
				{
					size_t m = 0;
					while (val[m])
						result[j++] = val[m++];
				}
				// free(key);
				i += varlen;
			}
            else if (ft_isdigit(str[i]))
            {
                i++;
            }
			else
			{
				result[j++] = '$';
				if (str[i])
					result[j++] = str[i++];
			}
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

static void	fix_token_types_after_expansion(t_token *tokens)
{
	t_token *curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD)
		{
			if (ft_strcmp(curr->value, "<<") == 0)
				curr->type = TOKEN_HEREDOC;
			else if (ft_strcmp(curr->value, "<") == 0)
				curr->type = TOKEN_REDIR_IN;
			else if (ft_strcmp(curr->value, ">>") == 0)
				curr->type = TOKEN_REDIR_APPEND;
			else if (ft_strcmp(curr->value, ">") == 0)
				curr->type = TOKEN_REDIR_OUT;
			else if (ft_strcmp(curr->value, "|") == 0)
				curr->type = TOKEN_PIPE;
		}
		curr = curr->next;
	}
}
void	expansion_all_tokens(t_token *tokens, t_env *env)
{
	t_token	*curr = tokens;

	fix_dollar_doublequote_tokens(&tokens);
	merge_variable_tokens(tokens);
	curr = tokens;

	while (curr)
	{
		if (is_in_heredoc(curr))
		{
			expansion_handle_heredoc(curr);
			fix_token_types_after_expansion(tokens);
			curr = curr->next;
			continue;
		}
        if ((curr->type == TOKEN_WORD || curr->type == TOKEN_VARIABLE) && curr->value)
		{
			char *expanded = expand_variables_in_string(curr->value, env);
			curr->value = expanded;
			curr->type = TOKEN_WORD;
		}
		if (curr->type == TOKEN_VARIABLE && curr->value && curr->value[0] == '$')
			expansion_handle_variable(curr, env);
		else if (curr->type == TOKEN_DQUOTE)
			expansion_handle_dquote(curr, env);
		else if (curr->type == TOKEN_SQUOTE)
			expansion_handle_squote(curr);
		else if (curr->type == TOKEN_WORD)
			expansion_handle_word(curr, env);
		curr = curr->next;
	}
}
