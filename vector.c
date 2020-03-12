/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 18:33:35 by karmand           #+#    #+#             */
/*   Updated: 2020/02/29 19:49:43 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
void		vect_set(t_vector *v, int x, int y, int z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

double		vect_norm(t_vector *v)
{
	return (sqrt(v->x * v->x + v->y * v->y + v->z * v->z));
}

void		vect_tonorm(t_vector *v)
{
	double	n;
	
	n = vect_norm(v);
	v->x /= n;
	v->y /= n;
	v->z /= n;
}

double		vect_dot(t_vector *u, t_vector *v)
{
	return (u->x * v->x + u->y * v->y + u->z * v->z);

}

t_vector	vect_vect(t_vector *u, t_vector *v)
{
	t_vector	w;

	w.x = u->y * v->z - u->z * v->y;
	w.y = u->z * v->x - u->x * v->z;
	w.z = u->x * v->y - u->y * v->x;
	return (w);
}

t_point		vect_translate(t_point *p, t_vector *v)
{
	t_point		r;

	r.x = p->x + v->x;
	r.y = p->y + v->y;
	r.z = p->z + v->z;
	return (r);
}

t_vector	vect_vector(t_point *p1, t_point *p2)
{
	t_vector	v;

	v.x = p2->x - p1->x;
	v.y = p2->y - p1->y;
	v.z = p2->z - p1->z;
	return (v); 
}

void		vect_add(t_vector *v1, t_vector *v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
	v1->z += v2->z;
}

int			vect_is_col(t_vector *v1, t_vector *v2)
{
	double	k;

	if (v2->x != 0)
	{
		k = v1->x / v2->x;
		if (v1->y == k * v2->y && v1->z == k * v2->z)
			return (1);
		return (0);
	}
	if (v1->x == 0)
	{
		if (v2->y != 0)
		{
			k = v1->y / v2->y;
			if (v1->z == k * v2->z)
				return (1);
		return (0);
		}
		if (v1->y == 0)
			return (1);
	}
	return (0);
}

t_vector	vect_def(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_vector	vect_mult(t_vector *v, double x)
{
	t_vector r;

	r.x = v->x * x;
	r.y = v->y * x;
	r.z = v->z * x;
	return (r);
}

t_point		vect_set_p(double x, double y, double z)
{
	t_point		p;

	p.x = x;
	p.y = y;
	p.z = z;
	return (p);
}
