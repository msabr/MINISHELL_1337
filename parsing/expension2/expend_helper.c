/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:01:27 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 22:19:18 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h" 

int	count_dollars(const char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] == '$')
	{
		count++;
		i++;
	}
	return (count);
}

char	*expand_many_dollars(const char *str, t_env *env)
{
	int			dollar_count;
	const char	*var_name;
	char		*val;
	int			i;

	dollar_count = count_dollars(str);
	i = dollar_count;
	var_name = str + i;
	if (var_name[0])
	{
		if (dollar_count % 2 == 1)
		{
			val = get_env_value(&env, var_name);
			if (val != NULL)
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
