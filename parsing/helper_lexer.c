/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalid <khalid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:00:00 by khalid            #+#    #+#             */
/*   Updated: 2025/06/28 00:00:00 by khalid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Skip whitespace characters and return new position
int skip_whitespace_chars(const char *input, int start)
{
    int i;
    
    if (!input)
        return (start);
    
    i = start;
    while (input[i] && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n'))
        i++;
    
    return (i);
}

// Check if character is a separator (space or operator start)
int is_separator_char(char c)
{
    if (c == ' ' || c == '\t' || c == '\n')
        return (1);
    if (c == '|' || c == '>' || c == '<')
        return (1);
    
    return (0);
}

// Detect operator at current position and return its length
int detect_operator_length(const char *input, int pos)
{
    if (!input || !input[pos])
        return (0);
    
    // Check 2-character operators first
    if (input[pos] == '>' && input[pos + 1] == '>')
        return (2);  // ">>"
    if (input[pos] == '<' && input[pos + 1] == '<')
        return (2);  // "<<"
    
    // Check 1-character operators
    if (input[pos] == '|' || input[pos] == '>' || input[pos] == '<')
        return (1);
    
    return (0);
}

// Extract operator string from input at position
char *extract_operator_string(const char *input, int pos, int length)
{
    char *operator;
    int i;
    
    if (!input || length <= 0)
        return (NULL);
    
    operator = malloc(sizeof(char) * (length + 1));
    if (!operator)
        return (NULL);
    
    i = 0;
    while (i < length && input[pos + i])
    {
        operator[i] = input[pos + i];
        i++;
    }
    operator[i] = '\0';
    
    return (operator);
}

// Find end of word (next separator or end of string)
int find_word_end(const char *input, int start)
{
    int i;
    
    if (!input)
        return (start);
    
    i = start;
    while (input[i] && !is_separator_char(input[i]))
        i++;
    
    return (i);
}

// Extract word from input between start and end positions
char *extract_word_string(const char *input, int start, int end)
{
    char *word;
    int length;
    int i;
    
    if (!input || start >= end)
        return (NULL);
    
    length = end - start;
    word = malloc(sizeof(char) * (length + 1));
    if (!word)
        return (NULL);
    
    i = 0;
    while (i < length && input[start + i])
    {
        word[i] = input[start + i];
        i++;
    }
    word[i] = '\0';
    
    return (word);
}

// Check if there was whitespace after current position
int has_space_after(const char *input, int current_end)
{
    if (!input || !input[current_end])
        return (0);
    
    return (input[current_end] == ' ' || input[current_end] == '\t' || input[current_end] == '\n');
}

// Main function to create token from extracted string
t_token *create_token_from_string(char *str, int space_after)
{
    t_type type;
    t_token *token;
    
    if (!str)
        return (NULL);
    
    type = classify_token_content(str);
    token = ft_lstnew_token(str, type, space_after);
    
    return (token);
}