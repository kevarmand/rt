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

void		vect_set(t_vec3 *v, int x, int y, int z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

double		vect_norm(t_vec3 *v)
{
	return (sqrt(v->x * v->x + v->y * v->y + v->z * v->z));
}

void		vect_tonorm(t_vec3 *v)
{
	double	n;

	n = vect_norm(v);
	if (n == 0)
	{
		error(ERR_NORM);
		n = 1;
	}
	v->x /= n;
	v->y /= n;
	v->z /= n;
}

double		vect_dot(t_vec3 *u, t_vec3 *v)
{
	return (u->x * v->x + u->y * v->y + u->z * v->z);
}

t_vec3	vect_vect(t_vec3 *u, t_vec3 *v)
{
	t_vec3	w;

	w.x = u->y * v->z - u->z * v->y;
	w.y = u->z * v->x - u->x * v->z;
	w.z = u->x * v->y - u->y * v->x;
	return (w);
}
