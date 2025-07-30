/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 10:11:50 by msabr             #+#    #+#             */
/*   Updated: 2025/07/30 13:43:56 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <stdio.h>

extern int	g_status;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE,
	TOKEN_VARIABLE,
	TOKEN_EOF
}	t_token_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			export_variable;
	struct s_env	*next;
}	t_env;

typedef struct s_heredoc
{
	char	*delimiter;
	int		fd_read;
	int		fd_write;
	char	*tmp_file;
	int		flag;
	t_env	**env;
}	t_heredoc;

typedef struct s_redir
{
	t_token_type	type;
	t_heredoc		*heredoc;
	char			*filename;
	int				fd_in;
	int				fd_out;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	bool			in_pipe;
	struct s_cmd	*next;
}	t_cmd;

void	configure_environment(t_env **env_list, char **env_array);
void	main_loop(t_env **env_list);
char	*get_env_value(t_env **env_list, const char *key);
int		*ft_get_status(void);
void	ft_set_status(int status);
t_cmd	*parse_input(char *input, t_env *env_list, int *status);
char	*expand_heredoc_content(const char *str, t_env **env,
			const char *del);

#endif
