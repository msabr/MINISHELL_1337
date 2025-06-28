/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kabouelf <kabouelf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 10:37:14 by kabouelf          #+#    #+#             */
/*   Updated: 2025/06/28 10:37:32 by kabouelf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Pointeur statique vers la tête de la liste t_gc
static t_gc **ft_env_gc(void)
{
    static t_gc *gc = NULL;
    return &gc;
}

// Alloue de la mémoire et l'enregistre dans le garbage collector
void *ft_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        return NULL;
    t_gc *new = malloc(sizeof(t_gc));
    if (!new)
    {
        free(ptr);
        return NULL;
    }
    new->ptr = ptr;
    new->next = *ft_env_gc();
    *ft_env_gc() = new;
    return ptr;
}

// Variante pour les allocations liées à l'environnement
void *ft_malloc_env(size_t size)
{
    return ft_malloc(size); // Même logique que ft_malloc
}

// Libère toutes les allocations suivies par le garbage collector
void ft_free(void)
{
    t_gc *current = *ft_env_gc();
    t_gc *tmp;
    while (current)
    {
        tmp = current;
        current = current->next;
        free(tmp->ptr);
        free(tmp);
    }
    *ft_env_gc() = NULL; // Réinitialise la liste
}