#include "../../minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	return (0);
}

// bool is_operator(char c)
// {
// 	if (c == '|' || c == '<' || c == '>')
// 		return (1);
// 	return (0);
// }

// t_token_type get_operator_type(const char *s)
// {
// 	if (!ft_strncmp(s, "|", 1))
// 		return TOKEN_PIPE;
// 	if (!ft_strncmp(s, ">>", 2))
// 		return TOKEN_REDIR_APPEND;
// 	if (!ft_strncmp(s, "<<", 2))
// 		return TOKEN_HEREDOC;
// 	if (!ft_strncmp(s, "<", 1))
// 		return TOKEN_REDIR_IN;
// 	if (!ft_strncmp(s, ">", 1))
// 		return TOKEN_REDIR_OUT;
// 	return TOKEN_WORD;
// }

char	*ft_strndup(const char *src, size_t n)
{
	char	*dst;
	size_t	i;

	dst = (char *)ft_malloc(n + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}