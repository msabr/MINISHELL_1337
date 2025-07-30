/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expension.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:39:54 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 05:35:41 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

char	*expand_helper(const char *v_name, int d_cnt, t_env *env, t_token *curr)
{
	char	*val;
	char	*expanded;

	expanded = NULL;
	if (d_cnt % 2 == 1)
	{
		val = get_env_value(&env, v_name);
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
		expanded = ft_strdup(v_name);
		curr->expended = 0;
	}
	return (expanded);
}

static void	expansion_handle_variable(t_token *curr, t_env *env)
{
	char	*expanded;

	expanded = get_expanded_variable(curr, env);
	curr->value = expanded;
	curr->type = TOKEN_WORD;
}

static void	expansion_handle_word(t_token *curr, t_env *env)
{
	char	*expanded;

	expanded = expand_variables_in_word(curr->value, env);
	curr->value = expanded;
}

static void	handle_expansion_token(t_token *curr, t_env *env, t_token *tokens)
{
	char	*expanded;

	if (is_in_heredoc(curr))
	{
		expansion_handle_heredoc(curr);
		fix_token_types_after_expansion(tokens);
		return ;
	}
	if ((curr->type == TOKEN_WORD || curr->type == TOKEN_VARIABLE)
		&& curr->value)
	{
		expanded = expand_variables_in_string(curr->value, env);
		curr->value = expanded;
		curr->type = TOKEN_WORD;
	}
	if (curr->type == TOKEN_VARIABLE && curr->value
		&& curr->value[0] == '$')
		expansion_handle_variable(curr, env);
	else if (curr->type == TOKEN_DQUOTE)
		expand_dquote_token(curr, env);
	else if (curr->type == TOKEN_SQUOTE)
		expand_squote_token(curr);
	else if (curr->type == TOKEN_WORD)
		expansion_handle_word(curr, env);
}

void	expansion_all_tokens(t_token *tokens, t_env *env)
{
	t_token	*curr;

	fix_dollar_doublequote_tokens(&tokens);
	merge_variable_tokens(tokens);
	curr = tokens;
	while (curr)
	{
		handle_expansion_token(curr, env, tokens);
		if (curr)
			curr = curr->next;
	}
}
