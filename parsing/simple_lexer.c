/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalid <khalid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 00:00:00 by khalid            #+#    #+#             */
/*   Updated: 2025/06/28 00:00:00 by khalid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Main lexing function - converts input string to token list
t_token *tokenize_input_string(const char *input)
{
    t_token *token_list;
    int current_pos;
    int input_length;
    
    if (!input)
        return (NULL);
    
    token_list = NULL;
    current_pos = 0;
    input_length = ft_strlen(input);
    
    while (current_pos < input_length)
    {
        // Skip whitespace at current position
        current_pos = skip_whitespace_chars(input, current_pos);
        
        // If we reached end of string, break
        if (current_pos >= input_length)
            break;
        
        // Process next token (operator or word)
        current_pos = process_next_element(input, current_pos, &token_list);
        
        // If processing failed, clean up and return NULL
        if (current_pos == -1)
        {
            ft_free_tokens(token_list);
            return (NULL);
        }
    }
    
    return (token_list);
}

// Process single element (operator or word) and add to token list
int process_next_element(const char *input, int start_pos, t_token **token_list)
{
    int operator_len;
    int new_pos;
    
    if (!input || !token_list)
        return (-1);
    
    // Check if current position is an operator
    operator_len = detect_operator_length(input, start_pos);
    
    if (operator_len > 0)
    {
        // Process operator
        new_pos = handle_operator_token(input, start_pos, operator_len, token_list);
    }
    else
    {
        // Process word
        new_pos = handle_word_token(input, start_pos, token_list);
    }
    
    return (new_pos);
}

// Handle operator token creation and add to list
int handle_operator_token(const char *input, int pos, int op_length, t_token **token_list)
{
    char *operator_str;
    t_token *new_token;
    int space_after;
    int next_pos;
    
    if (!input || !token_list)
        return (-1);
    
    // Extract operator string
    operator_str = extract_operator_string(input, pos, op_length);
    if (!operator_str)
        return (-1);
    
    // Calculate next position and check for space after
    next_pos = pos + op_length;
    space_after = has_space_after(input, next_pos);
    
    // Create token with operator
    new_token = create_token_from_string(operator_str, space_after);
    if (!new_token)
    {
        free(operator_str);
        return (-1);
    }
    
    // Add to token list
    ft_lstadd_back_token(token_list, new_token);
    
    return (next_pos);
}

// Handle word token creation and add to list
int handle_word_token(const char *input, int start_pos, t_token **token_list)
{
    int word_end;
    char *word_str;
    t_token *new_token;
    int space_after;
    
    if (!input || !token_list)
        return (-1);
    
    // Find end of current word
    word_end = find_word_end(input, start_pos);
    
    // Extract word string
    word_str = extract_word_string(input, start_pos, word_end);
    if (!word_str)
        return (-1);
    
    // Check if there's space after this word
    space_after = has_space_after(input, word_end);
    
    // Create token with word
    new_token = create_token_from_string(word_str, space_after);
    if (!new_token)
    {
        free(word_str);
        return (-1);
    }
    
    // Add to token list
    ft_lstadd_back_token(token_list, new_token);
    
    return (word_end);
}

// Debug function to print all tokens (for testing)
void print_token_list(t_token *tokens)
{
    t_token *current;
    const char *type_names[] = {
        "SQUOTE", "SPCE", "DQUOTE", "VAR", "WORD", 
        "PIPE", "REDIR_IN", "REDIR_OUT", "HEREDOC", "APPEND"
    };
    
    current = tokens;
    printf("=== TOKEN LIST ===\n");
    
    while (current)
    {
        printf("Token: \"%s\" | Type: %s | Space_after: %d\n", 
               current->value, 
               type_names[current->type], 
               current->a_sp);
        current = current->next;
    }
    
    printf("=== END LIST ===\n");
}

// Main function for testing the lexer
int test_lexer_main(int argc, char **argv)
{
    t_token *tokens;
    int i;
    
    if (argc < 2)
    {
        printf("Usage: %s \"command line to tokenize\"\n", argv[0]);
        return (1);
    }
    
    // Test each argument
    for (i = 1; i < argc; i++)
    {
        printf("\n--- Testing input: \"%s\" ---\n", argv[i]);
        
        tokens = tokenize_input_string(argv[i]);
        if (tokens)
        {
            print_token_list(tokens);
            ft_free_tokens(tokens);
        }
        else
        {
            printf("Error: Failed to tokenize input\n");
        }
    }
    
    return (0);
}
int main(int ac, char **av)
{
    return(test_lexer_main(ac,av));
}