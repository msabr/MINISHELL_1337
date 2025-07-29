#include "../../minishell.h"

// Supprime les $[chiffre]
char *expand_numbers(const char *str)
{
    char *res = ft_strdup("");
    int i = 0;
    while (str[i])
    {
        if (str[i] == '$' && ft_isdigit(str[i + 1]))
            i += 2;
        else
        {
            char tmp[2] = {str[i], 0};
            char *tmp2 = strjoin_and_free(res, tmp);
            res = tmp2;
            i++;
        }
    }
    return res;
}