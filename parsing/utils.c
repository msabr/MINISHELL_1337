#include "../minishell.h"

int	*ft_get_status(void)
{
	static int	status;

	return (&status);
}

void	ft_set_status(int status)
{
	int	*ptr;

	ptr = ft_get_status();
	*ptr = status;
}