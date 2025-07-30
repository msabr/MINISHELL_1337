/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_utils5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:28:28 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 12:30:35 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

char	*extract_env_key2(const char *str, size_t i, size_t varlen)
{
	size_t	k;
	char	*key;

	key = ft_malloc(varlen);
	if (!key)
		return (NULL);
	k = 0;
	while (k < varlen - 1)
	{
		key[k] = str[i + 1 + k];
		k++;
	}
	key[varlen - 1] = '\0';
	return (key);
}

size_t	calc_exit_code_len(size_t *i)
{
	char	*exit_code;
	size_t	len;

	exit_code = ft_itoa(*ft_get_status());
	if (exit_code)
		len = ft_strlen(exit_code);
	else
		len = 0;
	*i += 2;
	return (len);
}

size_t	calc_other_dollar_len(const char *str, size_t *i)
{
	size_t	len;

	if (str[*i + 1])
	{
		*i += 2;
		len = 2;
	}
	else
	{
		(*i)++;
		len = 1;
	}
	return (len);
}

size_t	calc_expanded_len(const char *str, t_env *env)
{
	size_t	i;
	size_t	tmp_len;

	i = 0;
	tmp_len = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				tmp_len += calc_exit_code_len(&i);
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
				tmp_len += calc_var_env_len(str, &i, env);
			else if (ft_isdigit(str[i + 1]))
				i += 2;
			else
				tmp_len += calc_other_dollar_len(str, &i);
		}
		else
		{
			tmp_len++;
			i++;
		}
	}
	return (tmp_len);
}

void	append_exit_code(t_expand *xp)
{
	char	*exit_code;
	size_t	k;

	exit_code = ft_itoa(*ft_get_status());
	k = 0;
	while (exit_code && exit_code[k])
	{
		xp->result[xp->j++] = exit_code[k];
		k++;
	}
	xp->i++;
}
