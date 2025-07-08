#include "../../minishell.h"

// Gère les échappements dans les guillemets doubles (\" ou \$)
char *handle_escape_in_dquotes(const char *str, int *index)
{
    char *result = malloc(2);
    result[0] = str[*index + 1];
    result[1] = '\0';
    *index += 2;
    return result;
}

// Gère les échappements hors quotes (\, n'importe quel caractère)
char *handle_escape_unquoted(const char *str, int *index)
{
    char *result = malloc(2);
    result[0] = str[*index + 1];
    result[1] = '\0';
    *index += 2;
    return result;
}