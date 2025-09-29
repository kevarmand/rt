/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inter2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 16:12:28 by karmand           #+#    #+#             */
/*   Updated: 2020/03/05 23:31:50 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			test_triangle(t_vec3 *v1, t_vec3 *v2, t_vec3 *v)
{
	t_vec3	p1;
	t_vec3	p2;

	p1 = vect_vect(v1, v);
	p2 = vect_vect(v, v2);
	if (vect_dot(&p1, &p2) > 0.0001)
		return (1);
	return (0);
}

double		inter_triangle(t_point *p, t_vec3 *v, t_tr *tr)
{
	double		a;
	double		b;
	t_point		m;
	t_vec3	vec[4];

	a = vect_dot(v, &(tr->norm));
	if (a == 0)
		return (-1);
	b = tr->d + vect_dot((t_vec3 *)p, &(tr->norm));
	a = equa_premier(a, b);
	vec[3] = vect_mult(v, a);
	m = vect_translate(p, &(vec[3]));
	vec[0] = vect_vector(&(tr->p1), &(tr->p2));
	vec[1] = vect_vector(&(tr->p3), &(tr->p1));
	vec[2] = vect_vector(&(tr->p2), &(tr->p3));
	vec[3] = vect_vector(&(tr->p1), &m);
	if (test_triangle(&(vec[0]), &(vec[1]), &(vec[3])))
		return (-1);
	vec[3] = vect_vector(&(tr->p2), &m);
	if (test_triangle(&(vec[0]), &(vec[2]), &(vec[3])))
		return (-1);
	vec[3] = vect_vector(&(tr->p3), &m);
	if (test_triangle(&(vec[1]), &(vec[2]), &(vec[3])))
		return (-1);
	return (a);
}

void		inter_cy2(t_vec3 u[3], double d[5], t_cy *cy, t_point *p)
{
	u[0] = vect_mult(&(cy->dir), d[0]);
	vect_sub(&(u[1]), &(u[0]));
	d[0] = vect_dot(&(u[1]), &(u[1]));
	u[2] = vect_vector(&(cy->point), p);
	d[1] = vect_dot(&(u[2]), &(cy->dir));
	u[0] = vect_mult(&(cy->dir), d[1]);
	vect_sub(&(u[2]), &(u[0]));
	d[1] = 2 * vect_dot(&(u[1]), &(u[2]));
	d[2] = vect_dot(&(u[2]), &(u[2])) - (cy->rayon) * (cy->rayon);
	d[3] = equa_second(d[0], d[1], d[2]);
}

double		inter_cylinder(t_point *p, t_vec3 *v, t_cy *cy)
{
	double		d[5];
	t_vec3	u[3];
	t_point		p1;

	u[1] = *v;
	d[0] = vect_dot(v, &(cy->dir));
	inter_cy2(u, d, cy, p);
	if (d[3] <= 0)
		return (-1);
	u[0] = vect_mult(v, d[3]);
	p1 = vect_translate(p, &(u[0]));
	u[0] = vect_vector(&(cy->point), &p1);
	d[4] = vect_dot(&(u[0]), &(cy->dir));
	if (d[4] > 0 && d[4] <= cy->height)
		return (d[3]);
	d[4] = -d[1] / d[0] - d[3];
	d[3] = d[4];
	if (d[3] <= 0)
		return (-1);
	u[0] = vect_mult(v, d[3]);
	p1 = vect_translate(p, &(u[0]));
	u[0] = vect_vector(&(cy->point), &p1);
	d[4] = vect_dot(&(u[0]), &(cy->dir));
	return ((d[4] > 0 && d[4] <= cy->height) ? (d[3]) : (-1));
}
