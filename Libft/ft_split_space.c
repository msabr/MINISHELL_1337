/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_space.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:10:55 by msabr             #+#    #+#             */
/*   Updated: 2025/06/20 19:59:48 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str)
{
	int	i;
	int	count;

	if (!str)
		return (0);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (!ft_isspace(str[i]) && (i == 0 || ft_isspace(str[i - 1])))
			count++;
		i++;
	}
	return (count);
}

static int	fill_word(char *dest, const char *src, int start)
{
	int	i;

	i = 0;
	while (src[start] && !ft_isspace(src[start]))
	{
		dest[i] = src[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (start);
}

static int	is_sep(char const *str, int start)
{
	while (str[start] && ft_isspace(str[start]))
		start++;
	return (start);
}

char	**ft_split_space(char const *s)
{
	int		i;
	int		index;
	int		start;
	char	**pnt;

	if (!s)
		return (NULL);
	pnt = (char **)malloc(sizeof(char *) * (count_words(s) + 1));
	if (!pnt)
		return (NULL);
	i = 0;
	index = 0;
	while (index < count_words(s))
	{
		i = is_sep(s, i);
		start = i;
		while (s[i] && !ft_isspace(s[i]))
			i++;
		pnt[index] = (char *)malloc(sizeof(char) * (i - start + 1));
		if (!pnt[index])
			return (free_split(pnt));
		fill_word(pnt[index++], s, start);
	}
	pnt[count_words(s)] = NULL;
	return (pnt);
}
