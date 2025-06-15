/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:49:09 by msabr             #+#    #+#             */
/*   Updated: 2025/06/14 12:14:35 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strtok(char * str, const char* sep)
{
    static char *last;
    char *start;

    if (str == NULL)
        str = last;

    if (str == NULL)
        return NULL;

    // Skip leading delimiters
    while (*str && ft_strchr(sep, *str))
        str++;

    if (*str == '\0') {
        last = NULL;
        return NULL;
    }

    start = str;

    // Find the end of the token
    while (*str && !ft_strchr(sep, *str))
        str++;

    if (*str) {
        *str = '\0';  // Null-terminate the token
        last = str + 1;  // Move past the delimiter
    } else {
        last = NULL;  // No more tokens
    }

    return start;
}

