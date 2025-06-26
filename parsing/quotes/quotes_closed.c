#include <stdio.h>
#include <unistd.h>
#include <string.h>

int check_closedqt(const char *str)
{
    int single_quote = 0;
    int double_quote = 0;

    if (!str)
    {
        write(2, "minishell: error: null input string\n", 35);
        return 0;
    }

    while (*str)
    {
        if (*str == '\'' && !double_quote)
            single_quote = !single_quote;
        else if (*str == '"' && !single_quote)
            double_quote = !double_quote;
        str++;
    }

    if (single_quote)
    {
        // ft_putstr_fd("minishell: syntax error: unclosed single quote\n", 2););
        write(2, "minishell: syntax error: unclosed single quote\n", 47);
        return 0;
    }
    if (double_quote)
    {
        // ft_putstr_fd("minishell: syntax error: unclosed double quote\n", 2);
        write(2, "minishell: syntax error: unclosed double quote\n", 48);
        return 0;
    }
    return 1;
}
