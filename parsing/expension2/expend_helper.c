# include "../../minishell.h" 
/*
** Expansion intelligente d’un token de type $...
*/
void	expand_var_token(t_token *token, t_env *env)
{
	if (token->value[1] == '?')
		convert_exit_code(token);
	else if (ft_isdigit(token->value[1]))
		number_before_dollar(token);
	else if (token->value[1] != '\0')
		expand_env_dollar(token, env);
}

/*
** Helpers internes pour expansion dans les doubles quotes
*/

static int	find_question(const char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			return (i);
		i++;
	}
	return (-1);
}

static int	find_number(const char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && ft_isdigit(str[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

static int	find_dollar(const char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			return (i);
		i++;
	}
	return (-1);
}

static char	*get_variable(const char *str, int index)
{
	int	h = index + 1;
	int	len = 0;
	while (str[h + len] && (ft_isalnum(str[h + len]) || str[h + len] == '_'))
		len++;
	return (ft_substr(str, h, len));
}

static char	*get_value(t_env *env, const char *key_val)
{
	while (env)
	{
		if (ft_strcmp(env->key, key_val) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*replace_question(char *str, int index)
{
	char	*exit_code = ft_itoa(g_status);
	char	*prefix = ft_substr(str, 0, index);
	char	*suffix = ft_substr(str, index + 2, ft_strlen(str) - (index + 2));
	char	*temp = ft_strjoin(prefix, exit_code);
	char	*new_str = ft_strjoin(temp, suffix);
	//free(exit_code); free(prefix); free(suffix); free(temp);
	return (new_str);
}

static char	*replace_number(char *str, int index)
{
	char	*prefix = ft_substr(str, 0, index);
	char	*suffix = ft_substr(str, index + 2, ft_strlen(str) - (index + 2));
	char	*new_str = ft_strjoin(prefix, suffix);
	//free(prefix); free(suffix);
	return (new_str);
}

static char	*replace_dollar(char *str, int index, t_env *env)
{
	char	*key_val = get_variable(str, index);
	char	*val = get_value(env, key_val);
	if (!val)
		val = "";
	char	*prefix = ft_substr(str, 0, index);
	char	*suffix = ft_strdup(&str[index + ft_strlen(key_val) + 1]);
	char	*temp = ft_strjoin(prefix, val);
	char	*new_str = ft_strjoin(temp, suffix);
	//free(key_val); free(prefix); free(suffix); free(temp);
	return (new_str);
}

/*
** Expansion dans les doubles quotes (gère $VAR, $?, $1 etc.)
*/
void	expand_dquote_token(t_token *token, t_env *env)
{
	char *result = remove_dquotes(token->value);
	int index;
	while ((index = find_question(result)) != -1)
	{
		// char *tmp = result;
		result = replace_question(result, index);
		// free(tmp);
	}
	while ((index = find_number(result)) != -1)
	{
		// char *tmp = result;
		result = replace_number(result, index);
		// free(tmp);
	}
	while ((index = find_dollar(result)) != -1)
	{
		// char *tmp = result;
		result = replace_dollar(result, index, env);
		// free(tmp);
	}
	// free(token->value);
	token->value = result;
	token->type = TOKEN_WORD;
}

char	*remove_squotes(char *str)
{
	size_t	len = ft_strlen(str);
	size_t	i, j;
	char	*res;

	if (str[0] == '\'' && len > 1 && str[len - 1] == '\'')
	{
		res = ft_malloc(len - 1);
		if (!res)
			return (NULL);
		for (i = 1, j = 0; i < len - 1; i++)
			res[j++] = str[i];
		res[j] = 0;
		// free(str);
		return (res);
	}
	return (str);
}

void	expand_squote_token(t_token *token)
{
	token->value = remove_squotes(token->value);
	token->type = TOKEN_WORD;
}
#include "expansion.h"
#include <stdlib.h>

void	convert_exit_code(t_token *token)
{
	char	*exit_code = ft_itoa(g_status);
	// free(token->value);
	token->value = exit_code;
}

void	number_before_dollar(t_token *token)
{
	// free(token->value);
	token->value = ft_strdup("");
	token->expended = 1;
}

void	expand_env_dollar(t_token *token, t_env *env)
{
	char	*key_val, *val = NULL;
	int		i = 1, len = 0;
	while (token->value[i] && (ft_isalnum(token->value[i]) || token->value[i] == '_'))
		len++, i++;
	key_val = ft_strndup(token->value + 1, len);
	while (env)
	{
		if (ft_strcmp(env->key, key_val) == 0)
		{
			val = ft_strdup(env->value);
			break;
		}
		env = env->next;
	}
	// free(token->value);
	if (!val)
	{
		token->value = ft_strdup("");
		token->expended = 1;
	}
	else
	{
		token->value = val;
		token->expended = 0;
	}
	// free(key_val);
}

char	*remove_dquotes(char *str)
{
	size_t	len = ft_strlen(str);
	size_t	i, j;
	char	*res;

	if (str[0] == '"' && len > 1 && str[len - 1] == '"')
	{
		res = ft_malloc(len - 1);
		if (!res)
			return (NULL);
		for (i = 1, j = 0; i < len - 1; i++)
			res[j++] = str[i];
		res[j] = 0;
		// free(str);
		return (res);
	}
	return (str);
}


void fix_dollar_doublequote_tokens(t_token **head) {
    t_token *token = *head;
    while (token && token->next) {

        if (token->type == TOKEN_VARIABLE &&
            token->value && token->value[0] == '$' &&
            token->next->type == TOKEN_DQUOTE)
        {
            t_token *dquote_token = token->next;
            t_token *to_remove = token;

            // Relier la liste
            if (to_remove->prev)
                to_remove->prev->next = dquote_token;
            dquote_token->prev = to_remove->prev;
            if (*head == to_remove)
                *head = dquote_token;

            // On peut transformer le DQUOTE en WORD pour la suite
            dquote_token->type = TOKEN_WORD;

            token = dquote_token; // continuer sur la suite
        } else {
            token = token->next;
        }
    }
}


char *expand_many_dollars(const char *str, t_env *env)
{
    int i = 0;
    int dollar_count = 0;

    // Compte le nombre de $
    while (str[i] == '$')
    {
        dollar_count++;
        i++;
    }
    const char *var_name = str + i;

    if (var_name[0])
    {
        if (dollar_count % 2 == 1) // impair : expansion de la variable
        {
            char *val = get_env_value(&env, var_name); // <-- adresse du pointeur
            if (val)
                return ft_strdup(val);
            else
                return ft_strdup("");
        }
        else // pair : littéral
        {
            return ft_strdup(var_name);
        }
    }
    // Cas où il n’y a pas de variable, juste des dollars
    return ft_strdup(str);
}
void merge_variable_tokens(t_token *tokens)
{
    t_token *curr = tokens;
    while (curr)
    {
        // Si le token actuel est VARIABLE
        if (curr->type == TOKEN_VARIABLE)
        {
            t_token *next = curr->next;
            // Tant que le suivant est aussi VARIABLE et pas d'espace
            while (next && next->type == TOKEN_VARIABLE && curr->space_after == 0)
            {
                char *tmp = ft_strjoin(curr->value, next->value);
                // free(curr->value);
                curr->value = tmp;

                // retire next de la liste chaînée
                curr->next = next->next;
                if (next->next)
                    next->next->prev = curr;
                // free(next->value);
                // free(next);
                next = curr->next;
            }
        }
        curr = curr->next;
    }
}

char	*expand_variables_in_word(char *str, t_env *env)
{
	size_t	i = 0;
	size_t	j = 0;
	char	*result;
	size_t	len = ft_strlen(str);
	result = ft_malloc(len * 2 + 32);
	if (!result)
		return (NULL);

	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			// Cas $?
			if (str[i] == '?')
			{
				char *exit_code = ft_itoa(g_status);
				size_t k = 0;
				while (exit_code && exit_code[k])
					result[j++] = exit_code[k++];
				// free(exit_code);
				i++;
			}
			// Cas $VAR classique (alpha/underscore puis alphanum/underscore)
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				size_t varlen = 0;
				while (str[i + varlen] && (ft_isalnum(str[i + varlen]) || str[i + varlen] == '_'))
					varlen++;
				char *key = ft_malloc(varlen + 1);
				size_t k = 0;
				while (k < varlen)
				{
					key[k] = str[i + k];
					k++;
				}
				key[k] = '\0';
				char *val = get_env_value(&env, key);
				if (val)
				{
					size_t m = 0;
					while (val[m])
						result[j++] = val[m++];
				}
				// sinon rien (var inconnue : string vide)
				// free(key);
				i += varlen;
			}
			// Cas $ suivi d'autre chose (ex: chiffre, isolé, etc.)
			else
			{
				result[j++] = '$';
				result[j++] = str[i++];
			}
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}

void    merge_collapsed_tokens(t_token *tokens)
{
    t_token *curr = tokens;
    while (curr && curr->next)
    {
        if ((curr->type == TOKEN_WORD || curr->type == TOKEN_VARIABLE)
            && (curr->next->type == TOKEN_WORD || curr->next->type == TOKEN_VARIABLE)
            && curr->space_after == 0)
        {
            char    *tmp = ft_strjoin(curr->value, curr->next->value);
            // free(curr->value);
            curr->value = tmp;

            t_token *to_rm = curr->next;
            curr->space_after = to_rm->space_after;
            curr->next = to_rm->next;
            // free(to_rm->value);
            // free(to_rm);
        }
        else
            curr = curr->next;
    }
}

