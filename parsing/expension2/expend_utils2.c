/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:03:37 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 19:13:55 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

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

	key_val = get_variable(str, index);
	val = get_value(env, key_val);
	if (!val)
		val = "";
	prefix = ft_substr(str, 0, index);
	suffix = ft_strdup(&str[index + ft_strlen(key_val) + 1]);
	temp = ft_strjoin(prefix, val);
	return (ft_strjoin(temp, suffix));
}
