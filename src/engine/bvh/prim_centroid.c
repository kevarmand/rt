/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prim_centroid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:05:45 by norivier          #+#    #+#             */
/*   Updated: 2025/12/10 04:05:17 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "scene.h"
#include "types.h"
#include "vector.h"

FORCEINLINE
static inline t_vec3f	triangle_centroid(t_triangle t)
{
	return ((t.v0 + t.v0 + t.v0 + t.edge2 + t.edge1) * RCP_3);
}

FORCEINLINE
static inline t_vec3f	sphere_centroid(t_sphere s)
{
	return (s.center);
}

FORCEINLINE
static inline t_vec3f	cylinder_centroid(t_cylinder c)
{
	return (c.base + c.axis * c.height * RCP_2);
}

FORCEINLINE
static inline t_vec3f	torus_centroid(t_torus t)
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
	return ((t_vec3f){0, 0, 0, 0});
}
