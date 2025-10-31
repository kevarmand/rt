/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prim_centroid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:05:45 by norivier          #+#    #+#             */
/*   Updated: 2025/10/31 02:06:23 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "bvh.h"

FORCEINLINE
extern inline t_vec3f	triangle_centroid(t_triangle t)
{
	t_vec3f	out;
	t_vec3f	v1;
	t_vec3f	v2;

	v1 = vec3f_add(t.v0, t.edge1);
	v2 = vec3f_add(t.v0, t.edge2);
	out.x = (t.v0.x + v1.x + v2.x) / 3.0f;
	out.y = (t.v0.y + v1.y + v2.y) / 3.0f;
	out.z = (t.v0.z + v1.z + v2.z) / 3.0f;
	return (out);
}

FORCEINLINE
extern inline t_vec3f	sphere_centroid(t_sphere s)
{
	return (s.center);
}

FORCEINLINE
extern inline t_vec3f	cylinder_centroid(t_cylinder c)
{
	t_vec3f	out;

	out.x = (c.p0.x + c.p1.x) / 2.0f;
	out.y = (c.p0.y + c.p1.y) / 2.0f;
	out.z = (c.p0.z + c.p1.z) / 2.0f;
	return (out);
}

FORCEINLINE
extern inline t_vec3f	torus_centroid(t_torus t)
{
	return (t.center);
}

FORCEINLINE
extern inline t_vec3f	prim_centroid(t_primitive *p)
{
	if (p->type == PRIM_TRIANGLE)
		return (triangle_centroid(p->tr));
	else if (p->type == PRIM_SPHERE)
		return (sphere_centroid(p->sp));
	else if (p->type == PRIM_CYLINDER)
		return (cylinder_centroid(p->cy));
	else if (p->type == PRIM_TORUS)
		return (torus_centroid(p->to));
	return ((t_vec3f){0, 0, 0});
}
