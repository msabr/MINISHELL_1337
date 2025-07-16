
#include "builtins.h"

// void	append_env_value(t_env **env_list, char *key, char *value)
// {
// 	t_env	*current;
// 	char	*new_value;
// 	char	*new_key;

// 	new_key = ft_malloc(ft_strlen(key));
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







#include "../../minishell.h"

// Helper: check if a variable name is a valid identifier
static int is_valid_identifier(const char *key) 
{
	if (!key || !key[0] || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	for (int i = 1; key[i]; i++)
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
	return 1;
}

// Helper: print export error
static void print_export_error(const char *arg) {
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	ft_set_status(1);
}

// Helper: check if an arg is an option (starts with -)
static int is_option(const char *arg) {
	return (arg && arg[0] == '-' && arg[1]);
}

// Helper: detect VAR+=val, returns index of '+', -1 otherwise
static int find_plus_equal(const char *arg) {
	for (int i = 1; arg[i]; i++) {
		if (arg[i] == '+' && arg[i+1] == '=')
			return i;
		if (arg[i] == '=')
			break;
	}
	return -1;
}

// Helper: extract key (and append flag)
static char *get_key(const char *arg, int *is_append) {
	int idx = find_plus_equal(arg);
	char *eq = ft_strchr(arg, '=');
	if (idx != -1) {
		*is_append = 1;
		return ft_strndup(arg, idx);
	}
	*is_append = 0;
	if (eq)
		return ft_strndup(arg, eq - arg);
	return ft_strdup(arg);
}

// Affiche les variables exportées triées
static void print_sorted_export(t_env *env) {
	int count = size_of_env_list(env);
	t_env **arr = ft_malloc(sizeof(t_env*) * count);
	t_env *cur = env;
	int i = 0;
	while (cur) {
		arr[i++] = cur;
		cur = cur->next;
	}
	// Bubble sort (simple)
	for (int a = 0; a < count - 1; a++) {
		for (int b = 0; b < count - a - 1; b++) {
			if (ft_strcmp(arr[b]->key, arr[b+1]->key) > 0) {
				t_env *tmp = arr[b]; arr[b] = arr[b+1]; arr[b+1] = tmp;
			}
		}
	}
	for (int j = 0; j < count; j++) {
		if (arr[j]->export_variable) {
			ft_putstr_fd("export ", 1);
			ft_putstr_fd(arr[j]->key, 1);
			if (arr[j]->value) {
				ft_putstr_fd("=\"", 1);
				ft_putstr_fd(arr[j]->value, 1);
				ft_putstr_fd("\"\n", 1);
			} else
				ft_putstr_fd("\n", 1);
		}
	}
	// free(arr);
}

// Ajoute/modifie une variable d'environnement (append ou non)
static void set_env_value(t_env **env_list, const char *key, const char *value, int append) {
	t_env *node = find_env_node((char*)key, *env_list);
	if (append) {
		if (node && node->value) {
			// char *old = node->value;
			node->value = ft_strjoin(node->value, value ? value : "");
			// free(old);
		} else {
			add_env_value(env_list, (char*)key, (char*)value);
		}
	} else {
		add_env_value(env_list, (char*)key, (char*)value);
	}
}

// Marque une variable comme exportée mais sans valeur
static void mark_export_only(t_env **env_list, const char *key) {
	t_env *node = find_env_node((char*)key, *env_list);
	if (!node)
		add_env_value(env_list, (char*)key, NULL);
	else
		node->export_variable = 1;
}

// Fonction principale export
void export(t_cmd *cmd, t_env **env_list) {
	if (!cmd->args[1]) {
		print_sorted_export(*env_list);
		ft_set_status(0);
		return;
	}
	for (int i = 1; cmd->args[i]; i++) {
		char *arg = cmd->args[i];
		int is_append = 0;
		if (!arg || !arg[0]) {
			print_export_error(arg ? arg : "");
			continue;
		}
		if (is_option(arg)) {
		   print_export_error(arg);
			continue;
		}
		char *eq = ft_strchr(arg, '=');
		char *key = get_key(arg, &is_append);

		// Validation stricte : pas de split sur espaces, pas de caractères spéciaux, ni quotes
		if (!is_valid_identifier(key)) {
			print_export_error(arg);
			// free(key);
			continue;
		}

		// Si '=' présent, c'est une affectation (append ou non), sinon juste export
		if (eq) {
			const char *value = eq + 1;
			set_env_value(env_list, key, value, is_append);
		} else {
			mark_export_only(env_list, key);
		}
		// free(key);
	}
	ft_set_status(0);
}



// static int	is_valid_export_key(const char *str)
// {
// 	if (!str || !*str)
// 		return (0);
// 	if (!ft_isalpha(*str) && *str != '_')
// 		return (0);
// 	while (*str)
// 	{
// 		if (!ft_isalnum(*str) && *str != '_')
// 			return (0);
// 		str++;
// 	}
// 	return (1);
// }

// static void	print_export_error(const char *arg)
// {
// 	ft_putstr_fd("export: `", STDERR_FILENO);
// 	ft_putstr_fd(arg, STDERR_FILENO);
// 	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
// }

// static char	*expand_export_value(const char *value, t_env *env)
// {
// 	size_t	len;

// 	if (!value)
// 		return (ft_strdup(""));
// 	len = ft_strlen(value);
// 	if (len >= 2 && value[0] == '\'' && value[len - 1] == '\'')
// 		return (ft_substr(value, 1, len - 2));
// 	return (expand_variables_in_word((char *)value, env));
// }

// static int	key_has_quote(const char *key)
// {
// 	int	k;

// 	k = 0;
// 	while (key && key[k])
// 	{
// 		if (key[k] == '"' || key[k] == '\'')
// 			return (1);
// 		k++;
// 	}
// 	return (0);
// }

// static void	handle_no_equal(char *key, t_env **env_list)
// {
// 	if (!find_env_node(key, *env_list))
// 		add_temporary_env_value(env_list, key);
// }

// static void	handle_append_case(t_env **env_list, char *key, char *expanded_value)
// {
// 	char	*old;
// 	char	*joined;

// 	old = get_env_value(env_list, key);
// 	joined = ft_strjoin(old ? old : "", expanded_value ? expanded_value : "");
// 	add_env_value(env_list, key, joined);
// 	free(joined);
// }

// static void	handle_set_case(t_env **env_list, char *key, char *expanded_value)
// {
// 	add_env_value(env_list, key, expanded_value ? expanded_value : "");
// }

// static int	parse_export_key(char *arg, char **key)
// {
// 	int		eq;

// 	eq = 0;
// 	while (arg[eq] && arg[eq] != '='
// 		&& !(arg[eq] == '+' && arg[eq + 1] == '='))
// 		eq++;
// 	*key = ft_substr(arg, 0, eq);
// 	return (eq);
// }

// static void	process_export_arg(
// 	t_cmd *cmd, t_env **env_list, int i)
// {
// 	char	*key;
// 	char	*raw_value;
// 	char	*expanded_value;
// 	int		eq;
// 	int		append;
// 	int		has_equal;

// 	eq = parse_export_key(cmd->args[i], &key);
// 	if (!key || !is_valid_export_key(key) || key_has_quote(key))
// 	{
// 		print_export_error(cmd->args[i]);
// 		free(key);
// 		return ;
// 	}
// 	append = 0;
// 	has_equal = 0;
// 	if (cmd->args[i][eq] == '+' && cmd->args[i][eq + 1] == '=')
// 	{
// 		append = 1;
// 		has_equal = 1;
// 		raw_value = cmd->args[i] + eq + 2;
// 	}
// 	else if (cmd->args[i][eq] == '=')
// 	{
// 		has_equal = 1;
// 		raw_value = cmd->args[i] + eq + 1;
// 	}
// 	else
// 		raw_value = NULL;
// 	if (!has_equal)
// 		handle_no_equal(key, env_list);
// 	else
// 	{
// 		expanded_value = expand_export_value(raw_value, *env_list);
// 		if (append)
// 			handle_append_case(env_list, key, expanded_value);
// 		else
// 			handle_set_case(env_list, key, expanded_value);
// 		free(expanded_value);
// 	}
// 	free(key);
// }

// void	export(t_cmd *cmd, t_env **env_list)
// {
// 	int	i;

// 	i = 1;
// 	if (!cmd->args || !cmd->args[i])
// 		return (export_withot_args(*env_list));
// 	while (cmd->args[i])
// 	{
// 		process_export_arg(cmd, env_list, i);
// 		i++;
// 	}
// }