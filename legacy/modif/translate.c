/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trnaslate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 16:16:15 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 17:09:10 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			translate_sphere(t_sp *sp, int type)
{
	t_vec3	v;

	v = translate_type(type);
	sp->centre = vect_translate(&(sp->centre), &v);
	return (SUCCESS);
}

int			translate_plane(t_pl *pl, int type)
{
	t_vec3	v;

	v = translate_type(type);
	pl->point = vect_translate(&(pl->point), &v);
	norm_pl(pl);
	return (SUCCESS);
}

int			translate_square(t_sq *sq, int type)
{
	t_vec3	v;

	v = translate_type(type);
	sq->point = vect_translate(&(sq->point), &v);
	sq->d = -(sq->a * sq->point.x);
	sq->d -= (sq->b * sq->point.y);
	sq->d -= (sq->c * sq->point.z);
	return (SUCCESS);
}

int			translate_cylinder(t_cy *cy, int type)
{
	t_vec3	v;

	v = translate_type(type);
	cy->point = vect_translate(&(cy->point), &v);
	return (SUCCESS);
}

int			translate_triangle(t_tr *tr, int type)
{
	t_vec3	v;

	v = translate_type(type);
	tr->p1 = vect_translate(&(tr->p1), &v);
	tr->p2 = vect_translate(&(tr->p2), &v);
	tr->p3 = vect_translate(&(tr->p3), &v);
	norm_tr(tr);
	return (SUCCESS);
}
