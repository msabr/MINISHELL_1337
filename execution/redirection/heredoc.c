/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 18:09:58 by msabr             #+#    #+#             */
/*   Updated: 2025/07/08 18:15:28 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../minishell.h"

// int	valide_delimiter(int type)
// {
// 	return (type == STRING || type == SINGLE_Q || type == DOUBLE_Q
// 		|| type == VAR || type == DOUBLE_DLR || type == EXIT_STATUS);
// }

// char	*ft_delimiter(t_token_node **debut, int *flag)
// {
// 	t_token_node	*cur;
// 	char			*delimiter;

// 	delimiter = NULL;
// 	cur = (*debut)->next;
// 	if (cur && cur->type == SPC)
// 		cur = cur->next;
// 	while (cur && valide_delimiter(cur->type))
// 	{
// 		if (!(*flag) && (cur->type == SINGLE_Q || cur->type == DOUBLE_Q))
// 			(*flag) = 42;
// 		if (!delimiter)
// 			delimiter = ft_strdup("");
// 		delimiter = ft_strjoin(delimiter, cur->value);
// 		cur = cur->next;
// 	}
// 	return (delimiter);
// }

// void	ft_herdoc(t_token_node *debut, t_env_var *env, t_minishell *data)
// {
// 	int				flag;
// 	char			*delimiter;
// 	t_token_node	*cur;

// 	cur = NULL;
// 	flag = 0;
// 	while (debut)
// 	{
// 		if (debut->type == HEREDOC)
// 		{
// 			if (verify_cmd(cur, NULL, false) == -1)
// 				break ;
// 			delimiter = ft_delimiter(&debut, &flag);
// 			if (!delimiter)
// 				break ;
// 			debut->fd_hrd = readline_hdc(delimiter, env, flag, data);
// 			if (debut->fd_hrd == -3)
// 			{
// 				data->exit_status = 1;
// 				return ;
// 			}
// 		}
// 		add_lst_back_token(&cur, new_token(debut->type, debut->value, -3));
// 		debut = debut->next;
// 	}
// }


// int	write_to_file(char *buffer)
// {
// 	char	*file_tmp;
// 	int		i;
// 	int		fd;
// 	int		fd_read;

// 	file_tmp = ft_strdup("hiba.txt");
// 	i = 0;
// 	while (access(file_tmp, F_OK) != -1)
// 		file_tmp = ft_strjoin("hiba.txt", ft_itoa(i++));
// 	fd = open(file_tmp, O_CREAT | O_RDWR, 0777);
// 	fd_read = open(file_tmp, O_RDWR, 0777);
// 	if (fd < 0)
// 		write(2, "Error\n", 6);
// 	if (fd_read < 0)
// 		write(2, "Error\n", 6);
// 	if (!buffer)
// 		buffer = ft_strdup("");
// 	write(fd, buffer, ft_strlen(buffer));
// 	close(fd);
// 	unlink(file_tmp);
// 	return (fd_read);
// }

// char	*add_cmd_dn_buffer(char *cmd, char *buffer)
// {
// 	if (!cmd)
// 		cmd = ft_strdup("");
// 	if (!buffer)
// 		buffer = ft_strdup("");
// 	buffer = ft_strjoin(buffer, cmd);
// 	buffer = ft_strjoin(buffer, ft_strdup("\n"));
// 	return (buffer);
// }

// int	readline_hdc(char *delimiter, t_env_var *env, int flag, t_minishell *data)
// {
// 	char	*cmd;
// 	char	*cmd_copy;
// 	char	*buffer;

// 	buffer = NULL;
// 	signal(SIGINT, handel_herdoc);
// 	while (1)
// 	{
// 		cmd = readline("> ");
// 		cmd_copy = cmd;
// 		if (!ttyname(0))
// 			return (free(cmd_copy), open(ttyname(2), O_RDWR), -3);
// 		if (!cmd)
// 			return (write_to_file(buffer));
// 		if (!ft_strcmp(cmd, delimiter))
// 		{
// 			free(cmd_copy);
// 			break ;
// 		}
// 		if (flag != 42)
// 			cmd = expand_vars_and_heredoc(cmd, env, data);
// 		buffer = add_cmd_dn_buffer(cmd, buffer);
// 		free(cmd_copy);
// 	}
// 	return (write_to_file(buffer));
// }
