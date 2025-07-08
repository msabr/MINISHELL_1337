/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:40:34 by msabr             #+#    #+#             */
/*   Updated: 2025/07/08 15:40:54 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
