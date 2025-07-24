/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 22:10:26 by msabr             #+#    #+#             */
/*   Updated: 2025/07/22 22:44:49 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void handle_heredoc_signal(int sig)
{
    if (sig == SIGINT)
        close(0);
}

static char *generate_unique_tmpfile(void)
{
    static int i = 0;
    char *base = ft_strdup("heredoc.txt");
    char *filename = NULL;
    while (access(base, F_OK) != -1)
    {
        free(base);
        filename = ft_strjoin("heredoc.txt", ft_itoa(i++));
        base = filename;
    }
    return base;
}

int write_to_tmpfile(char *buffer)
{
    char *file_tmp = generate_unique_tmpfile();
    int fd = open(file_tmp, O_CREAT | O_RDWR, 0777);
    int fd_read = open(file_tmp, O_RDWR, 0777);

    if (fd < 0 || fd_read < 0)
    {
        write(2, "Error\n", 6);
        free(file_tmp);
        return -1;
    }
    if (!buffer)
        buffer = ft_strdup("");
    write(fd, buffer, ft_strlen(buffer));
    close(fd);
    unlink(file_tmp);
    free(file_tmp);
    return fd_read;
}

char *add_cmd_to_buffer(char *cmd, char *buffer)
{
    char *new_buffer = NULL;
    char *tmp = NULL;

    if (!cmd)
        return buffer;
    if (!buffer || *buffer == '\0')
    {
        new_buffer = ft_strdup(cmd);
    }
    else
    {
        tmp = ft_strjoin(buffer, "\n");
        new_buffer = ft_strjoin(tmp, cmd);
        free(tmp);
    }
    free(buffer);
    return new_buffer;
}

char *expand_vars_and_heredoc(char *cmd, t_env *env)
{
    char *expanded_cmd;

    if (!cmd || !env)
        return cmd;
    expanded_cmd = expand_heredoc_content(cmd, &env, g_status, NULL);
    if (!expanded_cmd)
        return NULL;
    return expanded_cmd;
}

int readline_heredoc(const char *delimiter, t_env *env, int flag)
{
    char *buffer = ft_strdup("");
    char *cmd = NULL;
    int fd = 0;

    while (1)
    {
        cmd = readline("> ");
        if (!cmd)
            break;
        if (ft_strcmp(cmd, delimiter) == 0)
        {
            free(cmd);
            break;
        }
        if (flag == 0 && cmd)
        {
            char *expanded = expand_vars_and_heredoc(cmd, env);
            free(cmd);
            cmd = expanded;
        }
        buffer = add_cmd_to_buffer(cmd, buffer);
        free(cmd);
    }
    if (buffer && *buffer)
        fd = write_to_tmpfile(buffer);
    free(buffer);
    return fd;
}

int verify_cmd(t_cmd *cmd, bool is_builtin)
{
    if (!cmd || !cmd->args || !cmd->args[0])
    {
        perror("Command not found");
        return -1;
    }
    if (is_builtin)
    {
        if (execve_builtin(cmd, cmd->redirs->heredoc->env) < 0)
        {
            perror("Execution failed");
            return -1;
        }
    }
    else
    {
        if (execve(cmd->args[0], cmd->args, list_to_env(*cmd->redirs->heredoc->env)) < 0)
        {
            perror("Execution failed");
            return -1;
        }
    }
    return 0;
}

char *get_delimiter(t_cmd *cmd, int *flag)
{
    t_redir *redir;

    if (!cmd || !cmd->redirs || !cmd->redirs->heredoc)
    {
        *flag = 1;
        return NULL;
    }
    redir = cmd->redirs;
    while (redir && redir->type != TOKEN_HEREDOC)
        redir = redir->next;
    if (!redir || !redir->heredoc || !redir->heredoc->delimiter)
    {
        *flag = 1;
        return NULL;
    }
    *flag = 0;
    return ft_strdup(redir->heredoc->delimiter);
}

int redirect_heredoc(t_cmd *cmds, t_env *env)
{
    t_redir *redir;
    char *delimiter;
    int fd;
    int flag;

    if (!cmds || !cmds->redirs)
        return 1;
    redir = cmds->redirs;
    delimiter = get_delimiter(cmds, &flag);
    if (flag)
    {
        fprintf(stderr, "heredoc: missing delimiter\n");
        return 1;
    }
    fd = readline_heredoc(delimiter, env, redir->heredoc->flag);
    free(delimiter);
    if (fd < 0)
        return 1;
    cmds->redirs->heredoc->fd_read = fd;
    return 0;
}