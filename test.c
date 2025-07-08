/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:33:45 by msabr             #+#    #+#             */
/*   Updated: 2025/07/08 15:43:09 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

bool	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

bool	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long long ft_atoll(const char *str)
{
    int			sign;
    long long	result;

    sign = 1;
    result = 0;
    while (ft_isspace(*str))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (ft_isdigit(*str))
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return (result * sign);
}
//atoi
int	main()
{
    long long	num;

    num = ft_atoll("922337203685477580");
    printf("Converted number: %lld\n", num);
    return (0);
}