/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:01:27 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 07:11:17 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h" 

static void	append_exit_code1(t_expand *xp)
{
	char	*exit_code;
	size_t	k;

	exit_code = ft_itoa(g_status);
	k = 0;
	while (exit_code && exit_code[k])
	{
		xp->result[xp->j++] = exit_code[k];
		k++;
	}
	xp->i++;
}

void	append_other_dollar(t_expand *xp, char *str)
{
	xp->result[xp->j++] = '$';
	if (str[xp->i])
	{
		xp->result[xp->j++] = str[xp->i];
		xp->i++;
	}
}

void	handle_dollar(t_expand *xp, char *str, t_env *env)
{
	xp->i++;
	if (str[xp->i] == '?')
		append_exit_code1(xp);
	else if (ft_isalpha(str[xp->i]) || str[xp->i] == '_')
		append_env_var(xp, str, env);
	else
		append_other_dollar(xp, str);
}

char	*expand_variables_in_word(char *str, t_env *env)
{
	t_expand	xp;

	xp.i = 0;
	xp.j = 0;
	xp.len = ft_strlen(str);
	xp.result = ft_malloc(xp.len * 2 + 32);
	if (!xp.result)
		return (NULL);
	while (str[xp.i])
	{
		if (str[xp.i] == '$' && str[xp.i + 1])
			handle_dollar(&xp, str, env);
		else
			xp.result[xp.j++] = str[xp.i++];
	}
	xp.result[xp.j] = '\0';
	return (xp.result);
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
