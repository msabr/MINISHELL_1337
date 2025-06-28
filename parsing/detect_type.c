/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_type.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalid <khalid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:00:00 by khalid            #+#    #+#             */
/*   Updated: 2025/06/28 00:00:00 by khalid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_shell_operator(const char *str)
{
    if (!str)
        return (0);
    
    if (ft_strcmp(str, "|") == 0)
        return (1);
    if (ft_strcmp(str, ">") == 0)
        return (1);
    if (ft_strcmp(str, "<") == 0)
        return (1);
    if (ft_strcmp(str, ">>") == 0)
        return (1);
    if (ft_strcmp(str, "<<") == 0)
        return (1);
    
    return (0);
}


int is_shell_variable(const char *str)
{
    if (!str || str[0] == '\0')
        return (0);
    
    return (str[0] == '$');
}

int has_single_quotes(const char *str)
{
    size_t len;
    
    if (!str)
        return (0);
    
    len = ft_strlen(str);
    if (len < 2)
        return (0);
    
    return (str[0] == '\'' && str[len - 1] == '\'');
}


int has_double_quotes(const char *str)
{
    size_t len;
    
    if (!str)
        return (0);
    
    len = ft_strlen(str);
    if (len < 2)
        return (0);
    
    return (str[0] == '"' && str[len - 1] == '"');
}

int is_only_whitespace(const char *str)
{
    int i;
    
    if (!str)
        return (0);
    
    i = 0;
    while (str[i])
    {
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            return (0);
        i++;
    }
    
    return (i > 0);
}


t_type classify_token_content(const char *value)
{
    if (!value)
        return (WORD);
    if (ft_strcmp(value, "|") == 0)
        return (PIPE);
    if (ft_strcmp(value, ">") == 0)
        return (REDIR_OUT);
    if (ft_strcmp(value, "<") == 0)
        return (REDIR_IN);
    if (ft_strcmp(value, ">>") == 0)
        return (APPEND);
    if (ft_strcmp(value, "<<") == 0)
        return (HEREDOC);
    
    if (is_shell_variable(value))
        return (VAR);
    if (has_single_quotes(value))
        return (SQUOTE);
    if (has_double_quotes(value))
        return (DQUOTE);
    if (is_only_whitespace(value))
        return (SPCE);
    return (WORD);
}

t_type get_operator_category(const char *op)
{
    if (!op)
        return (WORD); 
    if (ft_strcmp(op, "|") == 0)
        return (PIPE);
    else if (ft_strcmp(op, ">") == 0)
        return (REDIR_OUT);
    else if (ft_strcmp(op, "<") == 0)
        return (REDIR_IN);
    else if (ft_strcmp(op, ">>") == 0)
        return (APPEND);
    else if (ft_strcmp(op, "<<") == 0)
        return (HEREDOC);
    
    return (WORD);
}

