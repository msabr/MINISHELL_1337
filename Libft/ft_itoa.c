/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:07:47 by msabr             #+#    #+#             */
/*   Updated: 2024/11/14 02:29:32 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digit(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int		count;
	char	*pnt;
	long	num;
	int		i;

	num = n;
	count = count_digit(n);
	pnt = (char *)malloc(sizeof(char) * (count + 1));
	if (!pnt)
		return (NULL);
	pnt[count] = '\0';
	if (num < 0)
	{
		pnt[0] = '-';
		num = -num;
	}
	i = count - 1;
	while (num > 0)
	{
		pnt[i--] = (num % 10) + '0';
		num /= 10;
	}
	if (n == 0)
		pnt[0] = '0';
	return (pnt);
}
