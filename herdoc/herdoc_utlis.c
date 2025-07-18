/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khalid058r <khalid058r@student.1337.ma>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:01:00 by khalid058r        #+#    #+#             */
/*   Updated: 2025/07/18 18:01:00 by khalid058r       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/* Handler for SIGINT in heredoc */
void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
		close(0); // Close STDIN to interrupt readline
	}
}

/* Vérifie si le token est un délimiteur heredoc spécial (quotes, etc) */
int	is_heredoc_delim(t_token *token)
{
	if (!token || !token->value)
		return (0);
	int	len = ft_strlen(token->value);
	if ((len == 1 && token->value[0] == '$') || len == 0)
	{
		if (token->next && (token->next->type == TOKEN_DQUOTE
				|| token->next->type == TOKEN_SQUOTE))
			return (1);
	}
	return (0);
}

/* Expansion: détecte $VAR dans une ligne heredoc */
int	find_dollar_heredoc(char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

/* Renvoie la clé de variable pour $VAR dans heredoc */
char	*get_variable_heredoc(char *str, int index)
{
	int	start = index + 1;
	int	len = 0;
	while (str[start + len] && ft_isalnum(str[start + len]))
		len++;
	return (ft_substr(str, start, len));
}

/* Cherche la valeur d'une variable dans l'env */
char	*get_value_heredoc(t_env **env, char *key_val)
{
	t_env *cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key_val) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

/* Remplace $VAR par sa valeur dans une ligne heredoc */
char	*replace_dollar_heredoc(char *str, int index, t_env **env)
{
	char	*prefix;
	char	*suffix;
	char	*key_val;
	char	*val;
	char	*temp;
	char	*result;

	key_val = get_variable_heredoc(str, index);
	prefix = ft_substr(str, 0, index);
	suffix = ft_strdup(&str[index + ft_strlen(key_val) + 1]);
	val = get_value_heredoc(env, key_val);
	if (!val)
		val = "";
	temp = ft_strjoin(prefix, val);
	result = ft_strjoin(temp, suffix);
	free(prefix); free(suffix); free(temp); free(key_val);
	free(str);
	return (result);
}

/* Expansion: détecte $? dans une ligne heredoc */
int	find_question_heredoc(char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
			return (i);
		i++;
	}
	return (-1);
}

/* Remplace $? par le code de retour dans une ligne heredoc */
char	*replace_question_heredoc(char *str, int index)
{
	char	*prefix;
	char	*suffix;
	char	exit_code[12];
	char	*temp;
	char	*result;

	snprintf(exit_code, sizeof(exit_code), "%d", g_status);
	prefix = ft_substr(str, 0, index);
	suffix = ft_strdup(&str[index + 2]);
	temp = ft_strjoin(prefix, exit_code);
	result = ft_strjoin(temp, suffix);
	free(prefix); free(suffix); free(temp);
	free(str);
	return (result);
}

/* Expansion: détecte $1, $2... dans une ligne heredoc */
int	find_number_heredoc(char *str)
{
	int	i = 0;
	while (str && str[i])
	{
		if (str[i] == '$' && ft_isdigit(str[i + 1]))
			return (i);
		i++;
	}
	return (-1);
}

/* Remplace $1, $2... par rien dans heredoc */
char	*replace_number_heredoc(char *str, int index)
{
	char	*prefix = ft_substr(str, 0, index);
	char	*suffix = ft_strdup(&str[index + 2]);
	char	*result = ft_strjoin(prefix, suffix);
	free(prefix); free(suffix);
	free(str);
	return (result);
}

/* Utilitaire: expansion complète d'une ligne heredoc */
char	*expand_heredoc_content(char *line, t_env **env)
{
	int idx;
	char *tmp = ft_strdup(line);
	while ((idx = find_dollar_heredoc(tmp)) >= 0)
		tmp = replace_dollar_heredoc(tmp, idx, env);
	while ((idx = find_number_heredoc(tmp)) >= 0)
		tmp = replace_number_heredoc(tmp, idx);
	while ((idx = find_question_heredoc(tmp)) >= 0)
		tmp = replace_question_heredoc(tmp, idx);
	return tmp;
}