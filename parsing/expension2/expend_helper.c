/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:01:27 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 14:15:51 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h" 

void	expand_var_token(t_token *token, t_env *env)
{
	if (token->value[1] == '?')
		convert_exit_code(token);
	else if (ft_isdigit(token->value[1]))
		number_before_dollar(token);
	else if (token->value[1] != '\0')
		expand_env_dollar(token, env);
}

int	find_question(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			return (i);
		i++;
	}
	return (-1);
}

int	find_dollar(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			return (i);
		i++;
	}
	return (-1);
}

int	find_number(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && ft_isdigit(str[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

char	*get_variable(const char *str, int index)
{
	int	h;
	int	len;

	h = index + 1;
	len = 0;
	while (str[h + len] && (ft_isalnum(str[h + len]) || str[h + len] == '_'))
		len++;
	return (ft_substr(str, h, len));
}

char	*get_value(t_env *env, const char *key_val)
{
	while (env)
	{
		if (ft_strcmp(env->key, key_val) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*replace_question(char *str, int index)
{
	char	*exit_code;
	char	*prefix;
	char	*suffix;
	char	*temp;
	char	*new_str;

	exit_code = ft_itoa(g_status);
	prefix = ft_substr(str, 0, index);
	suffix = ft_substr(str, index + 2, ft_strlen(str) - (index + 2));
	temp = ft_strjoin(prefix, exit_code);
	new_str = ft_strjoin(temp, suffix);
	return (new_str);
}

char	*replace_number(char *str, int index)
{
	char	*prefix;
	char	*suffix;
	char	*new_str;

	prefix = ft_substr(str, 0, index);
	suffix = ft_substr(str, index + 2, ft_strlen(str) - (index + 2));
	new_str = ft_strjoin(prefix, suffix);
	return (new_str);
}

char	*replace_dollar(char *str, int index, t_env *env)
{
	char	*key_val;
	char	*val;
	char	*prefix;
	char	*suffix;
	char	*temp;
	char	*new_str;

	key_val = get_variable(str, index);
	val = get_value(env, key_val);
	if (!val)
		val = "";
	prefix = ft_substr(str, 0, index);
	suffix = ft_strdup(&str[index + ft_strlen(key_val) + 1]);
	temp = ft_strjoin(prefix, val);
	new_str = ft_strjoin(temp, suffix);
	return (new_str);
}

void	expand_dquote_token(t_token *token, t_env *env)
{
	char	*result;
	int		index;

	result = remove_dquotes(token->value);
	index = find_question(result);
	while (index != -1)
	{
		result = replace_question(result, index);
		index = find_question(result);
	}
	index = find_number(result);
	while (index != -1)
	{
		result = replace_number(result, index);
		index = find_number(result);
	}
	index = find_dollar(result);
	while (index != -1)
	{
		result = replace_dollar(result, index, env);
		index = find_dollar(result);
	}
	token->value = result;
	token->type = TOKEN_WORD;
}

char	*remove_squotes(char *str)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*res;

	len = ft_strlen(str);
	i = 1;
	j = 0;
	if (str[0] == '\'' && len > 1 && str[len - 1] == '\'')
	{
		res = ft_malloc(len - 1);
		if (!res)
			return (NULL);
		while (i < len - 1)
		{
			res[j] = str[i];
			j++;
			i++;
		}
		res[j] = 0;
		return (res);
	}
	return (str);
}

void	expand_squote_token(t_token *token)
{
	token->value = remove_squotes(token->value);
	token->type = TOKEN_WORD;
}

void	convert_exit_code(t_token *token)
{
	char	*exit_code;

	exit_code = ft_itoa(g_status);
	token->value = exit_code;
}

void	number_before_dollar(t_token *token)
{
	token->value = strdup("");
	token->expended = 1;
}

void	expand_env_dollar(t_token *token, t_env *env)
{
	char	*key_val;
	char	*val;
	int		i;
	int		len;

	val = NULL;
	i = 1;
	len = 0;
	while (token->value[i]
		&& (ft_isalnum(token->value[i]) || token->value[i] == '_'))
	{
		len++;
		i++;
	}
	key_val = strndup(token->value + 1, len);
	while (env)
	{
		if (ft_strcmp(env->key, key_val) == 0)
		{
			val = strdup(env->value);
			break ;
		}
		env = env->next;
	}
	if (!val)
	{
		token->value = strdup("");
		token->expended = 1;
	}
	else
	{
		token->value = val;
		token->expended = 0;
	}
}

char	*remove_dquotes(char *str)
{
	size_t	len;
	size_t	i;
	size_t	j;
	char	*res;

	len = ft_strlen(str);
	i = 1;
	j = 0;
	if (str[0] == '"' && len > 1 && str[len - 1] == '"')
	{
		res = ft_malloc(len - 1);
		if (!res)
			return (NULL);
		while (i < len - 1)
		{
			res[j] = str[i];
			j++;
			i++;
		}
		res[j] = 0;
		return (res);
	}
	return (str);
}

void	fix_dollar_doublequote_tokens(t_token **head)
{
	t_token	*token;
	t_token	*dquote_token;
	t_token	*to_remove;

	token = *head;
	while (token && token->next)
	{
		if (token->type == TOKEN_VARIABLE
			&& token->value && token->value[0] == '$'
			&& token->next->type == TOKEN_DQUOTE)
		{
			dquote_token = token->next;
			to_remove = token;
			if (to_remove->prev)
				to_remove->prev->next = dquote_token;
			dquote_token->prev = to_remove->prev;
			if (*head == to_remove)
				*head = dquote_token;
			dquote_token->type = TOKEN_WORD;
			token = dquote_token;
		}
		else
			token = token->next;
	}
}

char	*expand_many_dollars(const char *str, t_env *env)
{
	int			i;
	int			dollar_count;
	const char	*var_name;
	char		*val;

	i = 0;
	dollar_count = 0;
	while (str[i] == '$')
	{
		dollar_count++;
		i++;
	}
	var_name = str + i;
	if (var_name[0])
	{
		if (dollar_count % 2 == 1)
		{
			val = get_env_value(&env, var_name);
			if (val)
				return (ft_strdup(val));
			else
				return (ft_strdup(""));
		}
		else
		{
			return (ft_strdup(var_name));
		}
	}
	return (ft_strdup(str));
}

void	merge_variable_tokens(t_token *tokens)
{
	t_token	*curr;
	t_token	*next;
	char	*tmp;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_VARIABLE)
		{
			next = curr->next;
			while (next && next->type == TOKEN_VARIABLE
				&& curr->space_after == 0)
			{
				tmp = ft_strjoin(curr->value, next->value);
				curr->value = tmp;
				curr->next = next->next;
				if (next->next)
					next->next->prev = curr;
				next = curr->next;
			}
		}
		curr = curr->next;
	}
}

char	*expand_variables_in_word(char *str, t_env *env)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*result;
	char	*exit_code;
	char	*key;
	char	*val;
	size_t	k;
	size_t	varlen;
	size_t	m;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	result = ft_malloc(len * 2 + 32);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			if (str[i] == '?')
			{
				exit_code = ft_itoa(g_status);
				k = 0;
				while (exit_code && exit_code[k])
				{
					result[j++] = exit_code[k];
					k++;
				}
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				varlen = 0;
				while (str[i + varlen]
					&& (ft_isalnum(str[i + varlen]) || str[i + varlen] == '_'))
					varlen++;
				key = ft_malloc(varlen + 1);
				k = 0;
				while (k < varlen)
				{
					key[k] = str[i + k];
					k++;
				}
				key[k] = '\0';
				val = get_env_value(&env, key);
				if (val)
				{
					m = 0;
					while (val[m])
					{
						result[j++] = val[m];
						m++;
					}
				}
				i += varlen;
			}
			else
			{
				result[j++] = '$';
				result[j++] = str[i++];
			}
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

void	merge_collapsed_tokens(t_token *tokens)
{
	t_token	*curr;
	t_token	*to_rm;
	char	*tmp;

	curr = tokens;
	while (curr && curr->next)
	{
		if ((curr->type == TOKEN_WORD || curr->type == TOKEN_VARIABLE)
			&& (curr->next->type == TOKEN_WORD
				|| curr->next->type == TOKEN_VARIABLE)
			&& curr->space_after == 0)
		{
			tmp = ft_strjoin(curr->value, curr->next->value);
			curr->value = tmp;
			to_rm = curr->next;
			curr->space_after = to_rm->space_after;
			curr->next = to_rm->next;
		}
		else
		{
			curr = curr->next;
		}
	}
}
