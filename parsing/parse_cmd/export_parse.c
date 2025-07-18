#include "../../minishell.h"

/*
** Helper : Trouver le token associé à la valeur (quotes, etc.)
** Ici, tu relies la valeur (après =) au token correct via comparaison de la valeur
*/
// t_token *find_token_for_value(t_token *tokens, const char *value)
// {
// 	t_token *curr = tokens;
// 	while (curr)
// 	{
// 		if (curr->value && value && ft_strcmp(curr->value, value) == 0)
// 			return curr;
// 		curr = curr->next;
// 	}
// 	return NULL;
// }

/*
** Helper : Compte le nombre d'arguments
// */
//  int	count_args(char **args)
// {
// 	int	i = 0;
// 	if (!args)
// 		return (0);
// 	while (args[i])
// 		i++;
// 	return (i);
// }

/*
** Handler robuste de préparation des arguments export (norme 42)
** - Gère quotes, append, expansion, word splitting
** - Modifie cmd->args pour que export ne fasse que l'exécution
*/
// void	parse_export_handler(t_token *tokens, t_cmd *cmd, t_env *env)
// {
// 	int		i = 1;
// 	int		new_argc = 1;
// 	int		w;
// 	char	**new_args;
// 	char	*arg;
// 	char	*equal;
// 	int		append;
// 	char	*key;
// 	char	*value;
// 	char	*expanded;
// 	char	*final_arg;
// 	t_token	*val_token;
// 	char	**words;

// 	new_args = ft_calloc(count_args(cmd->args) * 4 + 4, sizeof(char *));
// 	if (!new_args)
// 		return ;
// 	new_args[0] = ft_strdup(cmd->args[0]); // "export"

// 	while (cmd->args[i])
// 	{
// 		arg = cmd->args[i];
// 		equal = ft_strchr(arg, '=');
// 		append = 0;
// 		key = NULL;
// 		value = NULL;

// 		if (equal && equal > arg && equal[-1] == '+')
// 			key = ft_substr(arg, 0, (equal - arg) - 1), append = 1;
// 		else if (equal)
// 			key = ft_substr(arg, 0, (equal - arg));
// 		else
// 			key = ft_strdup(arg);
// 		value = equal ? equal + 1 : NULL;

// 		val_token = find_token_for_value(tokens, value);

// 		if (value)
// 		{
// 			if (val_token && val_token->type == TOKEN_SQUOTE)
// 			{
// 				final_arg = ft_strjoin(key, append ? "+=" : "=");
// 				final_arg = strjoin_and_free(final_arg, value);
// 				new_args[new_argc++] = final_arg;
// 			}
// 			else if (val_token && val_token->type == TOKEN_DQUOTE)
// 			{
// 				expanded = expand_variables_in_word(value, env);
// 				final_arg = ft_strjoin(key, append ? "+=" : "=");
// 				final_arg = strjoin_and_free(final_arg, expanded);
// 				free(expanded);
// 				new_args[new_argc++] = final_arg;
// 			}
// 			else
// 			{
// 				expanded = expand_variables_in_word(value, env);
// 				if (has_special_chars(expanded))
// 				{
// 					words = split_words(expanded);
// 					w = 0;
// 					while (words && words[w])
// 					{
// 						final_arg = ft_strjoin(key, append ? "+=" : "=");
// 						final_arg = strjoin_and_free(final_arg, words[w]);
// 						new_args[new_argc++] = final_arg;
// 						w++;
// 					}
// 					free_words(words);
// 				}
// 				else
// 				{
// 					final_arg = ft_strjoin(key, append ? "+=" : "=");
// 					final_arg = strjoin_and_free(final_arg, expanded);
// 					new_args[new_argc++] = final_arg;
// 				}
// 				free(expanded);
// 			}
// 		}
// 		else
// 			new_args[new_argc++] = ft_strdup(key);

// 		free(key);
// 		i++;
// 	}
// 	new_args[new_argc] = NULL;
// 	free_words(cmd->args);
// 	cmd->args = new_args;
// }

// /*
// ** Helper : concatène et libère s1 (norme 42 : pas de leaks !)
// */
// char	*strjoin_and_free(char *s1, const char *s2)
// {
// 	char	*joined;

// 	joined = ft_strjoin(s1, s2);
// 	free(s1);
// 	return (joined);
// }