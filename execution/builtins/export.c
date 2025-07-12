/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:52:06 by msabr             #+#    #+#             */
/*   Updated: 2025/07/05 23:29:16 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

// static void	append_env_value(t_env **env_list, char *key, char *value)
// {
// 	t_env	*current;
// 	char	*new_value;
// 	char	*new_key;

// 	new_key = malloc(ft_strlen(key));
// 	ft_strlcpy(new_key, key, ft_strlen(key));
// 	current = *env_list;
// 	while (current)
// 	{
// 		if (ft_strcmp(current->key, new_key) == 0)
// 		{
// 			new_value = ft_strjoin(current->value, value);
// 			current->value = new_value;
// 			return ;
// 		}
// 		current = current->next;
// 	}
// 	add_env_value(env_list, new_key, value);
// }

// static void	print_error_export(char *arg)
// {
// 	ft_putstr_fd("export: `", STDERR_FILENO);
// 	ft_putstr_fd(arg, STDERR_FILENO);
// 	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
// }

// void	export(t_cmd *cmd, t_env **env_list)
// {
// 	int		i ;
// 	char	*key;
// 	char	*value;

// 	i = 1;
// 	if (!cmd->args || !cmd->args[i])
// 		return (export_withot_args(*env_list));
// 	while (cmd->args[i])
// 	{
// 		if (!is_valid_key_export(cmd->args[i]))
// 			print_error_export(cmd->args[i]);
// 		else if (ft_strchr(cmd->args[i], '=') != NULL)
// 		{
// 			key = set_key(cmd->args[i]);
// 			value = ft_strchr(cmd->args[i], '=') + 1;
// 			if (ft_strstr(cmd->args[i], "+="))
// 				append_env_value(env_list, key, value);
// 			else
// 				add_env_value(env_list, key, value);
// 		}
// 		else
// 			add_temporary_env_value(env_list, cmd->args[i]);
// 		i++;
// 	}
// }



#include "builtins.h"

// Vérifie si le nom de variable est valide pour export (POSIX strict)
static int	is_valid_export_key(const char *str)
{
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

// Affiche l’erreur export bash-like
static void	print_export_error(const char *arg)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

// Expansion quote-safe (pas d’expansion si quotes simples)
static char	*expand_export_value(const char *value, t_env *env)
{
	size_t	len;

	if (!value)
		return (ft_strdup(""));
	len = ft_strlen(value);
	if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'')
		return (ft_substr(value, 1, len - 2));
	return expand_variables_in_word((char *)value, env);
}

void	export(t_cmd *cmd, t_env **env_list)
{
	int		i = 1;
	int		eq, append, has_equal, k, quote_in_key;
	char	*key;
	char	*raw_value;
	char	*expanded_value;
	char	*old;
	char	*joined;

	if (!cmd->args || !cmd->args[i])
		return (export_withot_args(*env_list));
	while (cmd->args[i])
	{
		// Parsing clé/valeur/append
		eq = 0;
		while (cmd->args[i][eq] && cmd->args[i][eq] != '='
			&& !(cmd->args[i][eq] == '+' && cmd->args[i][eq + 1] == '='))
			eq++;
		key = ft_substr(cmd->args[i], 0, eq);

		// Vérifie les quotes dans le nom
		quote_in_key = 0;
		for (k = 0; key && key[k]; k++)
			if (key[k] == '"' || key[k] == '\'')
				quote_in_key = 1;

		if (!key || !is_valid_export_key(key) || quote_in_key)
		{
			print_export_error(cmd->args[i]);
			if (key)
				free(key);
			i++;
			continue;
		}
		append = 0;
		has_equal = 0;
		if (cmd->args[i][eq] == '+' && cmd->args[i][eq + 1] == '=')
		{
			append = 1;
			has_equal = 1;
			raw_value = cmd->args[i] + eq + 2;
		}
		else if (cmd->args[i][eq] == '=')
		{
			has_equal = 1;
			raw_value = cmd->args[i] + eq + 1;
		}
		else
			raw_value = NULL;

		if (!has_equal)
		{
			if (!find_env_node(key, *env_list))
				add_temporary_env_value(env_list, key);
			free(key);
			i++;
			continue;
		}
		expanded_value = expand_export_value(raw_value, *env_list);
		if (append)
		{
			old = get_env_value(env_list, key);
			joined = ft_strjoin(old ? old : "", expanded_value ? expanded_value : "");
			add_env_value(env_list, key, joined);
			free(joined);
		}
		else
		{
			add_env_value(env_list, key, expanded_value ? expanded_value : "");
		}
		free(key);
		if (expanded_value)
			free(expanded_value);
		i++;
	}
}