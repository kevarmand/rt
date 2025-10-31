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
static inline void	triangle_bound(t_triangle t, t_aabb out)
{
	t_vec3f	v1;
	t_vec3f	v2;

	v1 = vec3f_add(t.v0, t.edge1);
	v2 = vec3f_add(t.v0, t.edge2);
	out[0] = vec3f_min(vec3f_min(t.v0, v1), v2);
	out[1] = vec3f_max(vec3f_max(t.v0, v1), v2);
}

FORCEINLINE
static inline void	sphere_bound(t_sphere s, t_aabb out)
{
	out[0].x = s.center.x - s.radius;
	out[0].y = s.center.y - s.radius;
	out[0].z = s.center.z - s.radius;
	out[1].x = s.center.x + s.radius;
	out[1].y = s.center.y + s.radius;
	out[1].z = s.center.z + s.radius;
}

FORCEINLINE
static inline void	cylinder_bound(t_cylinder c, t_aabb out)
{
	out[0] = vec3f_min(c.p0, c.p1);
	out[1] = vec3f_max(c.p0, c.p1);
	out[0].x -= c.radius;
	out[0].y -= c.radius;
	out[0].z -= c.radius;
	out[1].x += c.radius;
	out[1].y += c.radius;
	out[1].z += c.radius;
}

FORCEINLINE
static inline void	torus_bound(t_torus t, t_aabb out)
{
	float	extent;

	extent = t.R + t.r;
	out[0].x = t.center.x - extent;
	out[0].y = t.center.y - extent;
	out[0].z = t.center.z - extent;
	out[1].x = t.center.x + extent;
	out[1].y = t.center.y + extent;
	out[1].z = t.center.z + extent;
}

FORCEINLINE
extern inline void	prim_bound(t_primitive *p)
{
	t_aabb	out;

	if (p->type == PRIM_TRIANGLE)
		triangle_bound(p->tr, out);
	else if (p->type == PRIM_SPHERE)
		sphere_bound(p->sp, out);
	else if (p->type == PRIM_CYLINDER)
		cylinder_bound(p->cy, out);
	else if (p->type == PRIM_TORUS)
		torus_bound(p->to, out);
	else
	{
		out[0] = (t_vec3f){0, 0, 0};
		out[1] = (t_vec3f){0, 0, 0};
	}
	p->bounds[0] = out[0];
	p->bounds[1] = out[1];
}
