/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dubaging.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:10:54 by kabouelf          #+#    #+#             */
/*   Updated: 2025/07/02 15:59:23 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static const char *token_type_name(t_token_type type) {
    switch (type) {
        case TOKEN_WORD:         return "WORD";
        case TOKEN_PIPE:         return "PIPE";
        case TOKEN_REDIR_IN:     return "REDIR_IN";
        case TOKEN_REDIR_OUT:    return "REDIR_OUT";
        case TOKEN_REDIR_APPEND: return "REDIR_APPEND";
        case TOKEN_HEREDOC:      return "HEREDOC";
        case TOKEN_SQUOTE:       return "SQUOTE";
        case TOKEN_DQUOTE:       return "DQUOTE";
        case TOKEN_VARIABLE:     return "VARIABLE";
        case TOKEN_EOF:          return "EOF";
        default:                 return "UNKNOWN";
    }
}

void print_token_list(t_token *list) {
    int i = 0;
    printf("=== TOKEN LIST ===\n");
    while (list) {
        printf("[%d] type: %-12s | value: \"%s\" | space_after: %d\n",
            i++,
            token_type_name(list->type),
            list->value ? list->value : "(null)",
            list->space_after
        );
        list = list->next;
    }
    printf("=== END OF LIST ===\n");
}
static void print_redirs(t_redir *r)
{
    while (r)
    {
        if (r->type == TOKEN_REDIR_IN)
            printf("  [redir]   <  %s\n", r->filename);
        else if (r->type == TOKEN_REDIR_OUT)
            printf("  [redir]   >  %s\n", r->filename);
        else if (r->type == TOKEN_REDIR_APPEND)
            printf("  [redir]  >>  %s\n", r->filename);
        else if (r->type == TOKEN_HEREDOC)
            printf("  [redir]  <<  %s\n", r->filename);
        r = r->next;
    }
}

static void print_heredocs(t_redir *h)
{
    while (h)
    {
        printf("  [heredoc] delimiter: %s\n", h->delimiter);
        if (h->heredoc_content)
            printf("    heredoc_content: %s\n", h->heredoc_content);
        h = h->next;
    }
}

void print_cmds(t_cmd *cmds)
{
    int i = 1;
    while (cmds)
    {
        printf("=== Command %d ===\n", i);
        printf("argv:");
        if (cmds->args)
        {
            for (int j = 0; cmds->args[j]; j++)
                printf(" [%s]", cmds->args[j]);
        }
        printf("\n");
        print_redirs(cmds->redirs);
        print_heredocs(cmds->redirs);
        if (cmds->next)
            printf("|\n");
        cmds = cmds->next;
        i++;
    }
    printf("=== End pipeline ===\n");
}