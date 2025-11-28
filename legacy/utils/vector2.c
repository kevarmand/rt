/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 18:33:35 by karmand           #+#    #+#             */
/*   Updated: 2020/02/29 19:49:43 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_point		vect_translate(t_point *p, t_vec3 *v)
{
	t_point		r;

	r.x = p->x + v->x;
	r.y = p->y + v->y;
	r.z = p->z + v->z;
	return (r);
}

t_vec3	vect_vector(t_point *p1, t_point *p2)
{
	t_vec3	v;

	v.x = p2->x - p1->x;
	v.y = p2->y - p1->y;
	v.z = p2->z - p1->z;
	return (v);
}

void		vect_add(t_vec3 *v1, t_vec3 *v2)
{
	v1->x += v2->x;
	v1->y += v2->y;
	v1->z += v2->z;
}

void		vect_sub(t_vec3 *v1, t_vec3 *v2)
{
	v1->x -= v2->x;
	v1->y -= v2->y;
	v1->z -= v2->z;
}

int			vect_is_col(t_vec3 *v1, t_vec3 *v2)
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
