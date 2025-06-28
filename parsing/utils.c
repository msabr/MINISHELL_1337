/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: binome <binome@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 00:00:00 by binome            #+#    #+#             */
/*   Updated: 2025/06/27 00:00:00 by binome           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t  ft_strlen(const char *s)
{
    size_t len;

    if (!s)
        return (0);
    len = 0;
    while (s[len])
        len++;
    return (len);
}

char    *ft_strdup(const char *s1)
{
    char    *dup;
    size_t  len;
    size_t  i;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    dup = malloc(len + 1);
    if (!dup)
        return (NULL);
    i = 0;
    while (i < len)
    {
        dup[i] = s1[i];
        i++;
    }
    dup[i] = '\0';
    return (dup);
}

int     ft_strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    if (!s1 || !s2 || n == 0)
        return (0);
    i = 0;
    while (i < n && (s1[i] || s2[i]))
    {
        if (s1[i] != s2[i])
            return ((unsigned char)s1[i] - (unsigned char)s2[i]);
        i++;
    }
    return (0);
}

int     ft_strcmp(const char *s1, const char *s2)
{
    size_t i;

    if (!s1 || !s2)
        return (0);
    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char    *ft_strjoin(char const *s1, char const *s2)
{
    char    *joined;
    size_t  len1;
    size_t  len2;
    size_t  i;
    size_t  j;

    if (!s1 || !s2)
        return (NULL);
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    joined = malloc(len1 + len2 + 1);
    if (!joined)
        return (NULL);
    i = 0;
    while (i < len1)
    {
        joined[i] = s1[i];
        i++;
    }
    j = 0;
    while (j < len2)
    {
        joined[i + j] = s2[j];
        j++;
    }
    joined[i + j] = '\0';
    return (joined);
}


int     ft_isalnum(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

int     ft_isdigit(int c)
{
    return (c >= '0' && c <= '9');
}

int     ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}
