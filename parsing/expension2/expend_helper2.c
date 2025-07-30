/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:59:27 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 12:30:39 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	append_env_var2(t_expand *xp, char *str, t_env *env)
{
	size_t	varlen;
	char	*key;
	char	*val;

	varlen = get_varlen(str, xp->i);
	key = get_env_key(str, xp->i, varlen);
	if (!key)
		return ;
	val = get_env_value(&env, key);
	append_env_value2(xp, val);
	xp->i += varlen;
}

static void	handle_dollar2(t_expand *xp, char *str, t_env *env)
{
	if (str[xp->i + 1] == '?')
	{
		xp->i++;
		append_exit_code(xp);
	}
	else if (ft_isalpha(str[xp->i + 1]) || str[xp->i + 1] == '_')
	{
		xp->i++;
		append_env_var2(xp, str, env);
	}
	else if (ft_isdigit(str[xp->i + 1]))
		xp->i += 2;
	else
	{
		xp->i++;
		append_other_dollar(xp, str);
	}
}

char	*expand_variables_in_string(const char *str, t_env *env)
{
	t_expand	xp;

	xp.i = 0;
	xp.j = 0;
	xp.len = calc_expanded_len(str, env);
	xp.result = ft_malloc(xp.len + 1);
	if (!xp.result)
		return (NULL);
	while (str[xp.i])
	{
		if (str[xp.i] == '$' && str[xp.i + 1])
			handle_dollar2(&xp, (char *)str, env);
		else
			xp.result[xp.j++] = str[xp.i++];
	}
	xp.result[xp.j] = '\0';
	return (xp.result);
}
