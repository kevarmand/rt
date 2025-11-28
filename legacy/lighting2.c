/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/02 18:22:53 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:27:04 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		light_tr(t_lighting *li, t_tr *tr)
{
	li->c_obj = tr->color;
	li->norm = tr->norm;
	return (SUCCESS);
}

int		light_sq(t_lighting *li, t_sq *sq)
{
	li->c_obj = sq->color;
	li->norm = sq->norm;
	return (SUCCESS);
}

int		light_pl(t_lighting *li, t_pl *pl)
{
	li->c_obj = pl->color;
	li->norm = pl->norm;
	return (SUCCESS);
}

int		light_cy(t_lighting *li, t_cy *cy, t_point *p)
{
	t_vec3	am;
	t_vec3	u;
	double		d;

	li->c_obj = cy->color;
	am = vect_vector(&(cy->point), p);
	d = vect_dot(&am, &(cy->dir));
	u = vect_mult(&(cy->dir), d);
	vect_sub(&am, &u);
	li->norm = am;
	return (SUCCESS);
}

int		light_sp(t_lighting *li, t_sp *sp, t_point *p)
{
	li->c_obj = sp->color;
	li->norm = vect_vector(&(sp->centre), p);
	vect_tonorm(&(li->norm));
	return (SUCCESS);
}
