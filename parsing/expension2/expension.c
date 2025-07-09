
# include "../../minishell.h"
/*
** Vérifie si le token est dans un heredoc (en regardant derrière les espaces)
*/
int	is_in_heredoc(t_token *token)
{
	t_token	*prev = token->prev;
	while (prev && prev->type == TOKEN_WORD && prev->space_after == false)
		prev = prev->prev;
	while (prev && prev->type == TOKEN_WORD && prev->space_after == true)
		prev = prev->prev;
	if (prev && prev->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

/*
** Expansion principale à appeler sur chaque liste de tokens (hors export)
*/

// void	expansion_all_tokens(t_token *tokens, t_env *env)
// {
// 	t_token	*curr = tokens;
//     fix_dollar_doublequote_tokens(&tokens);
//     merge_variable_tokens(tokens); // <--- ajoute ceci !

// 	while (curr)
// 	{
// 		if (is_in_heredoc(curr))
// 		{
// 			if (curr->type == TOKEN_DQUOTE)
// 				curr->value = remove_dquotes(curr->value);
// 			else if (curr->type == TOKEN_SQUOTE)
// 				curr->value = remove_squotes(curr->value);
// 			curr->type = TOKEN_WORD;
// 			curr = curr->next;
// 			continue;
// 		}
// 		if (curr->type == TOKEN_VARIABLE && curr->value && curr->value[0] == '$')
// 		{
//             char *expanded = expand_many_dollars(curr->value, env);			
//             free(curr->value);
// 			curr->value = expanded;
// 			curr->type = TOKEN_WORD;
// 		}
// 		else if (curr->type == TOKEN_DQUOTE)
// 			expand_dquote_token(curr, env);
// 		else if (curr->type == TOKEN_SQUOTE)
// 			expand_squote_token(curr);
// 		curr = curr->next;
// 	}
// }



void expansion_all_tokens(t_token *tokens, t_env *env)
{
    t_token *curr = tokens;
    fix_dollar_doublequote_tokens(&tokens);
    merge_variable_tokens(tokens); // IMPORTANT : voir la discussion précédente

    while (curr)
    {
        if (is_in_heredoc(curr))
        {
            if (curr->type == TOKEN_DQUOTE)
                curr->value = remove_dquotes(curr->value);
            else if (curr->type == TOKEN_SQUOTE)
                curr->value = remove_squotes(curr->value);
            curr->type = TOKEN_WORD;
            curr = curr->next;
            continue;
        }
        if (curr->type == TOKEN_VARIABLE && curr->value && curr->value[0] == '$')
        {
            // Expansion directe pair/impair
            int i = 0;
            int dollar_count = 0;
            while (curr->value[i] == '$')
            {
                dollar_count++;
                i++;
            }
            const char *var_name = curr->value + i;
            char *expanded = NULL;
            if (var_name[0])
            {
                if (dollar_count % 2 == 1) // impair : expansion
                {
                    char *val = get_env_value(&env, var_name);
                    if (val)
                        expanded = ft_strdup(val);
                    else
                        expanded = ft_strdup("");
                }
                else // pair : littéral
                {
                    expanded = ft_strdup(var_name);
                }
            }
            else
            {
                expanded = ft_strdup(curr->value);
            }
            free(curr->value);
            curr->value = expanded;
            curr->type = TOKEN_WORD;
        }
        else if (curr->type == TOKEN_DQUOTE)
            expand_dquote_token(curr, env);
        else if (curr->type == TOKEN_SQUOTE)
            expand_squote_token(curr);
        curr = curr->next;
    }
}