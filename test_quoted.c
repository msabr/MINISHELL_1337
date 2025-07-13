#include "minishell.h"

int main()
{
    // Test simple pour vérifier le champ quoted
    t_token *tokens = lexer2("echo 'hello world' \"test\" normal");
    
    printf("Test du champ quoted:\n");
    print_token_list(tokens);
    
    // Vérifier que les tokens quotés ont le bon flag
    t_token *curr = tokens;
    while (curr)
    {
        if (curr->type == TOKEN_SQUOTE || curr->type == TOKEN_DQUOTE)
        {
            printf("Token quoté trouvé: type=%d, quoted=%d, value=\"%s\"\n", 
                   curr->type, curr->quoted, curr->value);
        }
        curr = curr->next;
    }
    
    free_token_list(tokens);
    return 0;
}
