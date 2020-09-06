/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 16:12:28 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:31:50 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double		inter_plane(t_point *p, t_vector *v, t_pl *pl)
{
	double		a;
	double		b;

	a = vect_dot(v, &(pl->norm));
	if (a == 0)
		return (-1);
	b = pl->d + vect_dot((t_vector *)p, &(pl->norm));
	return (equa_premier(a, b));
}

double		inter_sphere(t_point *p, t_vector *v, t_sp *sp)
{
	double		a;
	double		b;
	double		c;
	t_vector	vt;

	a = vect_dot(v, v);
	vt = vect_vector(&(sp->centre), p);
	b = 2 * vect_dot(v, &vt);
	c = vect_dot(&vt, &vt) - (sp->rayon) * (sp->rayon);
	return (equa_second(a, b, c));
}

double		inter_square(t_point *p, t_vector *v, t_sq *sq)
{
	double		a;
	double		b;
	t_vector	u1;
	t_point		p1;

	a = vect_dot(v, &(sq->norm));
	if (a == 0)
		return (-1);
	b = sq->d + vect_dot((t_vector *)p, &(sq->norm));
	b = equa_premier(a, b);
	if (b < 0)
		return (-1);
	u1 = vect_mult(v, b);
	p1 = vect_translate(p, &u1);
	u1 = vect_vector(&p1, &(sq->point));
	a = vect_dot(&u1, &(sq->dir1));
	if (ft_abs(a) > sq->height)
		return (-1);
	a = vect_dot(&u1, &(sq->dir2));
	if (ft_abs(a) > sq->height)
		return (-1);
	return (b);
}

double		inter_switch(t_point *p, t_vector *v, t_plot *pl)
{
	if (pl->type == PLANE)
	{
		return (inter_plane(p, v, (t_pl *)(pl->p)));
	}
	if (pl->type == SPHERE)
	{
		return (inter_sphere(p, v, (t_sp *)(pl->p)));
	}
	if (pl->type == TRIANGLE)
	{
		return (inter_triangle(p, v, (t_tr *)(pl->p)));
	}
	if (pl->type == CYLINDER)
		return (inter_cylinder(p, v, (t_cy *)(pl->p)));
	if (pl->type == SQUARE)
		return (inter_square(p, v, (t_sq *)(pl->p)));
	return (0);
}

int			inter_inter(t_ray *r, t_ldist *l, int j)
{
	double	dmin;
	double	d;
	int		i;
	int		imin;

	dmin = 10000001;
	i = -1;
	imin = -1;
	while (++i < l->n)
	{
		d = inter_switch(&(r->source), &(r->dir), &(l->plot[i]));
		if (d < dmin && (d > 0.01 || (d > 0 && i != j)))
		{
			dmin = d;
			imin = i;
		}
	}
	i = j;
	r->inter = imin;
	r->dist = dmin;
	return (SUCCESS);
}
