/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msabr <msabr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:05:44 by msabr             #+#    #+#             */
/*   Updated: 2025/07/16 13:29:14 by msabr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct m_gc_node
{
	void				*ptr;
	struct m_gc_node	*next;
}	t_c_gc_node;






void	*ft_malloc(size_t size)
{
	void			*ptr;
	t_gc_node		*node;

	ptr = ft_malloc(size);
	if (!ptr)
		exit(1);
	node = ft_malloc(sizeof(t_gc_node));
	if (!node)
	{
		gc_free_all();
		exit(1);
	}
	node->ptr = ptr;
	node->next = gc->gc_env_list;
	return (ptr);
}

void	gc_free_all1(void)
{
	t_c_gc_node		*current;
	t_c_gc_node		*next;
	t_gc_manager	*gc;

	gc = get_gc();
	current = gc->gc_cmd_list;
	while (current)
	{
		next = current->next;
		if (current->ptr)
			free(current->ptr);
		free(current);
		current = next;
	}
	gc->gc_cmd_list = NULL;
}

void	gc_free_all(void)
{
	t_gc_node		*tmp;
	t_gc_manager	*gc;

	gc = get_gc();
	while (gc->gc_env_list)
	{
		tmp = gc->gc_env_list;
		free(gc->gc_env_list->ptr);
		gc->gc_env_list = gc->gc_env_list->next;
		free(tmp);
	}
}