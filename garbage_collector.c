/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oessoufi <oessoufi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:30:30 by oessoufi          #+#    #+#             */
/*   Updated: 2025/02/18 12:50:41 by oessoufi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lstclear_garbage(t_alloc **lst)
{
	t_alloc	*tmp;

	if (lst == NULL)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		free((*lst)->addr);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	ft_lstadd_front(t_alloc **lst, t_alloc *new)
{
	if (new == NULL || lst == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}

void	*ft_malloc(size_t size, t_data *data)
{
	void	*allocated;
	t_alloc	*new;

	new = malloc(sizeof(t_alloc));
	if (new == NULL)
	{
		ft_lstclear_garbage(&data->alloc);
		free(data);
		write(2, "ERROR : malloc couldnt allocate!", 33);
		exit(1);
	}
	allocated = malloc(size);
	if (allocated == NULL)
	{
		ft_lstclear_garbage(&data->alloc);
		free(data);
		free(new);
		write(2, "ERROR : malloc couldnt allocate!", 33);
		exit(1);
	}
	new->addr = allocated;
	new->next = NULL;
	ft_lstadd_front(&data->alloc, new);
	return(allocated);
}