/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:49:20 by msabr             #+#    #+#             */
/*   Updated: 2025/07/22 18:06:33 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *str, char c)
{
	int	i;
	int	count;

	if (!str)
		return (0);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != c && (i == 0 || str[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static int	fill_word(char *dest, const char *src, int start, char c)
{
	int	i;

	i = 0;
	while (src[start] && src[start] != c)
	{
		dest[i] = src[start];
		i++;
		start++;
	}
	dest[i] = '\0';
	return (start);
}

static int	is_sep(char const *str, char sep, int start)
{
	while (str[start] && str[start] == sep)
		start++;
	return (start);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		index;
	int		start;
	char	**pnt;

	if (!s)
		return (NULL);
	pnt = (char **)ft_malloc(sizeof(char *) * (count_words(s, c) + 1));
	i = 0;
	index = 0;
	while (index < count_words(s, c))
	{
		i = is_sep(s, c, i);
		start = i;
		while (s[i] && s[i] != c)
			i++;
		pnt[index] = (char *)ft_malloc(sizeof(char) * (i - start + 1));
		fill_word(pnt[index++], s, start, c);
	}
	pnt[count_words(s, c)] = NULL;
	return (pnt);
}
