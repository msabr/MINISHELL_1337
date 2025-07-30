/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:00:15 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/29 19:03:15 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	expand_var_token(t_token *token, t_env *env)
{
	if (token->value[1] == '?')
		convert_exit_code(token);
	else if (ft_isdigit(token->value[1]))
		number_before_dollar(token);
	else if (token->value[1] != '\0')
		expand_env_dollar(token, env);
}

int	find_question(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			return (i);
		i++;
	}
	return (-1);
}

int	find_dollar(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			return (i);
		i++;
	}
	return (-1);
}

int	find_number(const char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && ft_isdigit(str[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

char	*get_variable(const char *str, int index)
{
	int	h;
	int	len;

	h = index + 1;
	len = 0;
	while (str[h + len] && (ft_isalnum(str[h + len]) || str[h + len] == '_'))
		len++;
	return (ft_substr(str, h, len));
}
