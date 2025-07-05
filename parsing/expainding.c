#include "../minishell.h"

/*
** Recherche la valeur d'une variable d'environnement dans la liste chainée.
** Retourne "" si la variable n'existe pas.
*/
static const char	*exp_getenv(t_env **env, const char *key)
{
	t_env	*curr;

	if (!env || !key)
		return ("");
	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0 && curr->value)
			return (curr->value);
		curr = curr->next;
	}
	return ("");
}

/*
** Word splitting : collapse espaces multiples, ignore leading/trailing.
*/
static char	*exp_word_splitting(const char *str)
{
	char	*res;
	int		i;
	int		j;
	int		inword;

	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	inword = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
		{
			if (j > 0 && !inword)
				res[j++] = ' ';
			res[j++] = str[i];
			inword = 1;
		}
		else if (inword)
			inword = 0;
		i++;
	}
	res[j] = '\0';
	return (res);
}

/*
** Détection d'une assignation de type VAR=val.
*/
bool	is_assignment(const char *str)
{
	int	i;

	i = 0;
	if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (false);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (str[i] == '=' && i > 0);
}

/*
** Ajoute ou modifie une variable d'environnement.
*/
void	set_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	current = malloc(sizeof(t_env));
	current->key = ft_strdup(key);
	current->value = ft_strdup(value);
	current->export_variable = false;
	current->next = *env_list;
	*env_list = current;
}

/*
** Traite une assignation VAR=val.
*/
void	handle_assignment_if_needed(const char *str, t_env **env_list)
{
	int		i;
	char	key[256];
	char	*value;

	if (!is_assignment(str))
		return ;
	i = 0;
	while (str[i] && str[i] != '=' && i < 255)
	{
		key[i] = str[i];
		i++;
	}
	key[i] = '\0';
	if (str[i] == '=')
		value = (char *)(str + i + 1);
	else
		value = "";
	set_env_value(env_list, key, value);
}

/*
** Expansion d'un mot selon le contexte (quotes, $?, $VAR, ${VAR}, $$, etc.).
*/
char	*expand_word(const char *input, t_env **env, int last_status, int heredoc_mode)
{
	char	buf[4096];
	int		r;
	int		i;
	int		context;

	r = 0;
	i = 0;
	context = 0;
	if (heredoc_mode == 1)
		return (ft_strdup(input));
	while (input[i])
	{
		// Quotes simples
		if (input[i] == '\'' && context != 2)
		{
			context = (context == 1) ? 0 : 1;
			i++;
			continue;
		}
		// Quotes doubles
		if (input[i] == '"' && context != 1)
		{
			context = (context == 2) ? 0 : 2;
			i++;
			continue;
		}
		// Expansion (hors quotes simples)
		if (input[i] == '$' && context != 1)
		{
			// $?
			if (input[i + 1] == '?')
			{
				char	*tmp = ft_itoa(last_status);
				int		k = 0;
				while (tmp[k])
					buf[r++] = tmp[k++];
				free(tmp);
				i += 2;
				continue;
			}
			// $$
			else if (input[i + 1] == '$')
			{
				buf[r++] = '$';
				i += 2;
				continue;
			}
			// ${VAR}
			else if (input[i + 1] == '{')
			{
				int	j = i + 2;
				int	k = 0;
				char	var[256];

				while (input[j] && input[j] != '}' && k < 255)
					var[k++] = input[j++];
				var[k] = '\0';
				if (input[j] == '}')
					i = j + 1;
				else
					i = j;
				{
					const char *val = exp_getenv(env, var);
					int	l = 0;
					while (val[l])
						buf[r++] = val[l++];
				}
				continue;
			}
			// $VAR (ou $VAR123, ou $VAR collé)
			else if (ft_isalpha(input[i + 1]) || input[i + 1] == '_')
			{
				char	var[256];
				int		k = 0;
				int		j = i + 1;

				while (input[j] && (ft_isalnum(input[j]) || input[j] == '_') && k < 255)
					var[k++] = input[j++];
				var[k] = '\0';
				{
					const char *val = exp_getenv(env, var);
					int	l = 0;
					while (val[l])
						buf[r++] = val[l++];
				}
				i = j;
				continue;
			}
			else
			{
				buf[r++] = '$';
				i++;
				continue;
			}
		}
		buf[r++] = input[i++];
	}
	buf[r] = '\0';
	if (context == 0 && heredoc_mode == 0)
	{
		char *res = exp_word_splitting(buf);
		char *ret = ft_strdup(res);
		free(res);
		return (ret);
	}
	return (ft_strdup(buf));
}

/*
** Expansion sur toute la liste de tokens.
** Gère aussi les assignations type VAR=val (stocke dans l'env).
*/
void	expand_token_list(t_token *tokens, t_env **env, int last_status, int heredoc_mode)
{
	t_token	*curr;
	char	*new_val;

	curr = tokens;
	while (curr)
	{
		// D'abord gérer l'assignation (VAR=val)
		handle_assignment_if_needed(curr->value, env);

		if (curr->type == TOKEN_WORD || curr->type == TOKEN_VARIABLE)
		{
			new_val = expand_word(curr->value, env, last_status, heredoc_mode);
			free(curr->value);
			curr->value = new_val;
		}
		curr = curr->next;
	}
}