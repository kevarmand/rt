/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmand <karmand@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 18:33:35 by karmand           #+#    #+#             */
/*   Updated: 2020/02/29 19:49:43 by karmand          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
