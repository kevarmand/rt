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
	b = pl->d + vect_dot((t_vector *)p,&(pl->norm));
	return (equa_premier(a, b));
}

double		inter_circle(t_point *p, t_vector *v, t_ci *ci)
{
	double		a;
	t_vector	vu;
	
	a = inter_plane(p,v, (t_pl *)ci);
	if (a < 0)
		return (a);
	vu = vect_vector(p, &(ci->centre));
	if (vect_norm(&vu) < ci->rayon)
		return (a);
	return (-1);
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

int		test_triangle(t_vector *v1, t_vector *v2, t_vector *v)
{
	t_vector	p1;
	t_vector	p2;

	p1 = vect_vect(v1, v);
	p2 = vect_vect(v, v2);
	if (vect_dot(&p1, &p2) > 0)
		return (0);
	return (1);
}

double		inter_triangle(t_point *p, t_vector *v, t_tr *tr)
{
	double		a;
	double		b;
	t_point		m;
	t_vector	ab;
	t_vector	ac;
	t_vector	bc;
	t_vector	xm;
	
	a = vect_dot(v, &(tr->norm));
	if (a == 0)
		return (-1);
	b = tr->d + vect_dot((t_vector *)p,&(tr->norm));
	a = equa_premier(a, b);

	xm = vect_mult(v, a);
	m = vect_translate(p, &xm);
	ab = vect_vector(&(tr->p1),&(tr->p2));
	ac = vect_vector(&(tr->p1),&(tr->p3));
	bc = vect_vector(&(tr->p2),&(tr->p3));
	xm = vect_vector(&(tr->p1), &m);
	if (test_triangle(&ab, &ac, &xm))
		return (-1);
	xm = vect_vector(&(tr->p2), &m);
	if (!test_triangle(&ab, &bc, &xm))
		return (-1);
	xm = vect_vector(&(tr->p3), &m);
	if (test_triangle(&ac, &bc, &xm))
		return (-1);
	return (a);
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
	if (pl->type == CIRCLE)
	{
		return (inter_circle(p, v, (t_ci *)(pl->p)));
	}
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
		if (d < dmin && d > 0 && i != j)
		{
			dmin = d;
			imin = i;
		}
	}
	r->inter = imin;
	r->dist = dmin;
	return (SUCCESS);
}
