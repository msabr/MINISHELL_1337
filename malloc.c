#include "minishell.h"

void	*malloc(size_t __size)
{
    (void)__size;
    static int i = 0;
    
        if (i == 0)
        {
              return(malloc(__size));
        } // Avoid unused parameter warning
    return (NULL); // Placeholder for actual memory allocation logic
}