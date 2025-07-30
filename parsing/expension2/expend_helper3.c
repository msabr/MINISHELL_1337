/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_helper3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 05:26:22 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/30 06:14:59 by kabouelf         ###   ########.fr       */
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

char	*extract_env_key(char *str, t_expand *xp, size_t *varlen)
{
	size_t	k;
	size_t	l;
	char	*key;

	k = 0;
	if (!ft_isalpha(str[xp->i]) && str[xp->i] != '_')
	{
		*varlen = 0;
		return (NULL);
	}
	while (str[xp->i + k]
		&& (ft_isalnum(str[xp->i + k]) || str[xp->i + k] == '_'))
		k++;
	*varlen = k;
	key = ft_malloc(k + 1);
	if (!key)
		return (NULL);
	l = 0;
	while (l < k)
	{
		key[l] = str[xp->i + l];
		l++;
	}
	key[k] = '\0';
	return (key);
}

void	append_env_var(t_expand *xp, char *str, t_env *env)
{
	size_t	varlen;
	char	*key;
	char	*val;
	size_t	m;

	key = extract_env_key(str, xp, &varlen);
	if (!key || varlen == 0)
		return ;
	val = get_env_value(&env, key);
	if (val)
	{
		m = 0;
		while (val[m])
		{
			xp->result[xp->j++] = val[m];
			m++;
		}
	}
	xp->i += varlen;
}

size_t	calc_var_env_len(const char *str, size_t *i, t_env *env)
{
	size_t	varlen;
	char	*key;
	char	*val;
	size_t	len;

	varlen = 1;
	while (str[*i + varlen]
		&& (ft_isalnum(str[*i + varlen]) || str[*i + varlen] == '_'))
		varlen++;
	key = extract_env_key2(str, *i, varlen);
	if (!key)
		return (0);
	val = get_env_value(&env, key);
	if (val)
		len = ft_strlen(val);
	else
		len = 0;
	*i += varlen;
	return (len);
}
