/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   distance.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 14:32:22 by karmand           #+#    #+#             */
/*   Updated: 2020/02/29 22:16:06 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		dist_init(t_data *d, t_ldist *p)
{
	int		n;
	t_objlist	*l;

	l = d->lobj;
	n = list_count(d->lobj);
	if (n == 0)
		return (ERR_EMPTY_OBJ);
	p->n = n;
	if (!(p->plot = malloc(sizeof(t_plot) * n)))
		return (1 + error(ERR_MALLOC));
	while (l != 0)
	{
		p->plot[n - 1].p = (l->obj);
		p->plot[n - 1].type = l->type;
		p->plot[n - 1].dist = -1;
		l = l->next;
		n--;
	}
	return (SUCCESS);
}

int		dist_destroy(t_ldist *p)
{
	free(p->plot);
	return (0);
}
