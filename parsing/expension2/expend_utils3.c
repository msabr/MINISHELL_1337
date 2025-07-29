/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:16:21 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 19:20:25 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

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

void	expand_squote_token(t_token *token)
{
	token->value = remove_squotes(token->value);
	token->type = TOKEN_WORD;
}
