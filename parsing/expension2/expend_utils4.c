/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:21:07 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 19:29:28 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	number_before_dollar(t_token *token)
{
	token->value = ft_strdup("");
	token->expended = 1;
}

void	convert_exit_code(t_token *token)
{
	char	*exit_code;

	exit_code = ft_itoa(g_status);
	token->value = exit_code;
}

char	*get_env_dup_value(t_env *env, char *key_val)
{
	while (env)
	{
		if (ft_strcmp(env->key, key_val) == 0)
			return (ft_strdup(env->value));
		env = env->next;
	}
	return (NULL);
}

void	expand_env_dollar(t_token *token, t_env *env)
{
	char	*key_val;
	char	*val;
	int		i;
	int		len;

	(1) && (val = NULL, i = 1, len = 0);
	while (token->value[i]
		&& (ft_isalnum(token->value[i]) || token->value[i] == '_'))
	{
		len++;
		i++;
	}
	key_val = ft_strndup(token->value + 1, len);
	val = get_env_dup_value(env, key_val);
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
