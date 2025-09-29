/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 17:26:08 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:32:35 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		init_cy(t_cy *cy)
{
	vect_tonorm(&(cy->dir));
	return (SUCCESS);
}

int		init_pl(t_pl *pl)
{
	pl->a = pl->norm.x;
	pl->b = pl->norm.y;
	pl->c = pl->norm.z;
	if (pl->a == 0 && pl->b == 0 && pl->c == 0)
		return (ERR_NORM);
	pl->d = -(pl->a * pl->point.x);
	pl->d -= (pl->b * pl->point.y);
	pl->d -= (pl->c * pl->point.z);
	return (SUCCESS);
}

int		init_sq(t_sq *sq)
{
	t_vec3	v;

	sq->a = sq->norm.x;
	sq->b = sq->norm.y;
	sq->c = sq->norm.z;
	if (sq->a == 0 && sq->b == 0 && sq->c == 0)
		return (ERR_NORM);
	vect_set(&(sq->dir1), 1, 0, 0);
	vect_set(&(sq->dir2), 0, 1, 0);
	vect_set(&v, 0, 0, 1);
	if (vect_is_col(&v, &(sq->norm)))
		return (norm_sq(sq));
	sq->dir1 = vect_vect(&v, &(sq->norm));
	vect_tonorm(&(sq->dir1));
	sq->dir2 = vect_vect(&(sq->norm), &(sq->dir1));
	vect_tonorm(&(sq->dir2));
	return (norm_sq(sq));
}

int		init_sp(t_sp *sp)
{
	(void)sp;
	return (SUCCESS);
}

int		init_tr(t_tr *tr)
{
	t_vec3	v1;
	t_vec3	v2;

	v1 = vect_vector(&(tr->p1), &(tr->p2));
	v2 = vect_vector(&(tr->p1), &(tr->p3));
	tr->norm = vect_vect(&v1, &v2);
	tr->a = tr->norm.x;
	tr->b = tr->norm.y;
	tr->c = tr->norm.z;
	if (tr->a == 0 && tr->b == 0 && tr->c == 0)
		return (ERR_NORM);
	tr->d = -(tr->a * tr->p1.x);
	tr->d -= (tr->b * tr->p1.y);
	tr->d -= (tr->c * tr->p1.z);
	return (SUCCESS);
}
