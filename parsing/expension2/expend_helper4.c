/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_helper4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:29:28 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 06:15:47 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

size_t	get_varlen(char *str, size_t i)
{
	size_t	varlen;

	varlen = 0;
	while (str[i + varlen]
		&& (ft_isalnum(str[i + varlen]) || str[i + varlen] == '_'))
		varlen++;
	return (varlen);
}

char	*get_env_key(char *str, size_t i, size_t varlen)
{
	size_t	k;
	char	*key;

	key = ft_malloc(varlen + 1);
	if (!key)
		return (NULL);
	k = 0;
	while (k < varlen)
	{
		key[k] = str[i + k];
		k++;
	}
	key[varlen] = '\0';
	return (key);
}

void	fix_token_types_after_expansion(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
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

void	append_env_value2(t_expand *xp, char *val)
{
	size_t	m;

	m = 0;
	while (val && val[m])
	{
		xp->result[xp->j++] = val[m];
		m++;
	}
}

char	*get_expanded_variable(t_token *curr, t_env *env)
{
	int			dollar_count;
	const char	*var_name;

	dollar_count = count_dollars(curr->value);
	var_name = curr->value + dollar_count;
	if (var_name[0])
		return (expand_helper(var_name, dollar_count, env, curr));
	curr->expended = 0;
	return (ft_strdup(curr->value));
}
