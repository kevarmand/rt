/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 12:57:54 by karmand           #+#    #+#             */
/*   Updated: 2020/02/28 15:53:56 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		list_addback(t_list **l, void *p)
{
	t_list	*new;
	t_list	**cur;

	cur = l;
	if (!(new = malloc(sizeof(t_list))))
		return (ERROR_MLC);
	new->next = 0;
	new->obj = p;
	while (*cur != 0)
		cur = &((*cur)->next);
	(*cur) = new;
	return (SUCCESS);
}

int		list_addfront(t_list **l, void *p, t_type t)
{
	t_list	*n;

	if (!(n = malloc(sizeof(t_list))))
		return (ERROR_MALLOC);
	n->next = *l;
	n->obj = p;
	n->type = t;
	(*l) = n;
	return (SUCCESS);
}

int		list_count(t_list *l)
{
	int n;

	n = 0;
	while (l != 0)
	{
		l = l->next;
		n++;
	}
	return (n);
}
