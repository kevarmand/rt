/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 01:56:51 by norivier          #+#    #+#             */
/*   Updated: 2025/10/31 02:00:28 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "types.h"

FORCEINLINE
static inline t_aabb	triangle_bound(t_triangle t)
{
	t_aabb	out;
	t_vec3f	v1;
	t_vec3f	v2;

	v1 = vec3f_add(t.v0, t.edge1);
	v2 = vec3f_add(t.v0, t.edge2);
	out.b[0] = vec3f_min(vec3f_min(t.v0, v1), v2);
	out.b[1] = vec3f_max(vec3f_max(t.v0, v1), v2);
	return (out);
}

FORCEINLINE
static inline t_aabb	sphere_bound(t_sphere s)
{
	t_aabb	out;

	out.b[0].x = s.center.x - s.radius;
	out.b[0].y = s.center.y - s.radius;
	out.b[0].z = s.center.z - s.radius;
	out.b[1].x = s.center.x + s.radius;
	out.b[1].y = s.center.y + s.radius;
	out.b[1].z = s.center.z + s.radius;
	return (out);
}

FORCEINLINE
static inline t_aabb	cylinder_bound(t_cylinder c)
{
	t_aabb	out;

	out.b[0] = vec3f_min(c.p0, c.p1);
	out.b[1] = vec3f_max(c.p0, c.p1);
	out.b[0].x -= c.radius;
	out.b[0].y -= c.radius;
	out.b[0].z -= c.radius;
	out.b[1].x += c.radius;
	out.b[1].y += c.radius;
	out.b[1].z += c.radius;
	return (out);
}

FORCEINLINE
static inline t_aabb	torus_bound(t_torus t)
{
	float	extent;
	t_aabb	out;

	extent = t.R + t.r;
	out.b[0].x = t.center.x - extent;
	out.b[0].y = t.center.y - extent;
	out.b[0].z = t.center.z - extent;
	out.b[1].x = t.center.x + extent;
	out.b[1].y = t.center.y + extent;
	out.b[1].z = t.center.z + extent;
	return (out);
}

FORCEINLINE
extern inline t_aabb	prim_bound(t_primitive *p)
{
	t_aabb	out;

	if (p->type == PRIM_TRIANGLE)
		out = triangle_bound(p->tr);
	else if (p->type == PRIM_SPHERE)
		out = sphere_bound(p->sp);
	else if (p->type == PRIM_CYLINDER)
		out = cylinder_bound(p->cy);
	else if (p->type == PRIM_TORUS)
		out = torus_bound(p->to);
	else
	{
		out.b[0] = (t_vec3f){0, 0, 0};
		out.b[1] = (t_vec3f){0, 0, 0};
	}
	return (out);
}
