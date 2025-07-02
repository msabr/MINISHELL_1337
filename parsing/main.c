/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:54:04 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/02 14:22:22 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

// int main()
// {
//     char *line;
//     t_token *tokens;

//     while (1)
//     {
//         line = readline("minishell$ ");
//         if (!line)
//             break;
//         if (line[0])
//             add_history(line);
//         tokens = lexer2(line);
//         print_token_list(tokens);
//         if (check_syntax_errors(tokens, line))
//         {
//             free_token_list(tokens);
//             continue;
//         }
//         t_cmd *cmds = parse_tokens_to_cmds(tokens);
//         // print_cmds(cmds);
//         // Libération de la mémoire des tokens
//         free_token_list(tokens);

//     }
//     return 0;
// }

