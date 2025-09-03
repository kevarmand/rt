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
		return (ERR_MALLOC);
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
		return (ERR_MALLOC);
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

int		list_free(t_list *l)
{
	if (l->next != 0)
		list_free(l->next);
	free(l->obj);
	free(l);
	return (0);
}
