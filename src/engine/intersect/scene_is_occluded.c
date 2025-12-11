/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_is_occluded.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:53:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/10 19:49:42 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "rt_config.h"
#include "scene.h"
#include "bvh.h"
#include "attributes.h"

static int	scene_is_occluded_planes(const t_scene *scene,
			const t_ray *ray, float max_distance, t_hit *hit)
{
	int		plane_index;
	float	hit_distance;

	plane_index = 0;
	while (plane_index < scene->plane_count)
	{
		if (hit->kind == HIT_PLANE && hit->primitive_id == plane_index)
		{
			plane_index++;
			continue ;
		}
		if (inter_plane(&scene->planes[plane_index].pl,
				ray, &hit_distance))
		{
			if (hit_distance > EPSILON && hit_distance < max_distance + EPSILON)
				return (1);
		}
		plane_index++;
	}
	return (0);
}

static int	scene_is_occluded_primitives(const t_scene *scene,
			const t_ray *ray, float max_distance)
{
	int		primitive_index;
	float	hit_distance;

	primitive_index = 0;
	while (primitive_index < scene->primitive_count)
	{
		if (inter_primitive(&scene->primitives[primitive_index],
				ray, &hit_distance))
		{
			if (hit_distance > 0.01f && hit_distance < max_distance)
				return (1);
		}
		primitive_index++;
	}
	return (0);
}

FORCEINLINE
extern inline int	sphere_ocult(t_ray r, t_sphere *s, t_hit *hit)
{
	t_vec3f			ro;
	t_vec3f			rd;
	t_equ			eq;
	float			roots[2];
	int				nroots;

	// ro = mat3x3_mulv(s->inv_basis, vec3f_sub(r.origin, s->center));
	// rd = mat3x3_mulv(s->inv_basis, r.dir);
	ro = vec3f_sub(r.origin, s->center);
	rd = r.dir;
	eq.a = 1.0f;
	eq.b = 2.0f * vec3f_dot(rd, ro);
	eq.c = vec3f_dot(ro, ro) - s->r_squared;
	nroots = solve_quad(eq, roots);
	if (nroots == 0)
		return (0);
	hit->t = -1.0f;
	if (roots[0] > -EPSILON && nroots > 1 && roots[1] > EPSILON)
		hit->t = roots[1];
	else
		return (0);
	return (1);
}

FORCEINLINE
extern inline int	ocult_inter(t_ray r, t_primitive *p, t_hit *out)
{
	if (p->type == PRIM_SPHERE)
		return (sphere_ocult(r, &p->sp, out));
	// else if (p->type == PRIM_CYLINDER)
	// 	return (cylinder_ocult(r, &p->cy, out));
	// else if (p->type == PRIM_TORUS)
	// 	return (torus_ocult(r, &p->to, out));
	return (0);
}

int	bvh_ocult(t_ray r, t_bvhnode *nodes, t_primitive *prims, t_hit *out)
{
	uint32_t	stack[64];
	int			sp;
	float		tmin;

	// out->t = FLT_MAX;
	sp = 0;
	stack[sp++] = 0;
	if (out->kind == HIT_PLANE || prims[out->primitive_id].type == PRIM_TRIANGLE)
		tmin = 0.0f;
	else
		tmin = TMIN_SHADOW;
	while (sp > 0)
	{
		uint32_t	idx = stack[--sp];
		t_bvhnode	*node = &nodes[idx];
		float		tnear;
		float		tfar;
		if (bound_intersect(r, node->bounds, &tnear, &tfar) == 0)
			continue ;
		if (tnear > out->t)
			continue ;
		if (node->is_leaf)
		{
			uint32_t	i;

			i = 0;
			while (i < node->leaf.count)
			{
				int	prim_id = node->leaf.start + i;
				t_hit	local_hit = *out;
				if (prim_inter(r, &prims[prim_id], &local_hit) != 0)
				{
					if (local_hit.t > tmin && local_hit.t < out->t)
						return (1);
				}
				i += 1;
			}
			continue ;
		}
		int	hitl; // so close
		int	hitr;
		float	tnearr;
		float	tfarr;

		hitl = bound_intersect(r, nodes[node->node.left].bounds, &tnear, &tfar);
		hitr = bound_intersect(r, nodes[node->node.right].bounds, &tnearr, &tfarr);
		if (hitl && tnear < out->t)
		{
			if (hitr && tnearr < out->t)
			{
				if (tnear < tnearr)
				{
					stack[sp++] = node->node.right;
					stack[sp++] = node->node.left;
				}
				else
				{
					stack[sp++] = node->node.left;
					stack[sp++] = node->node.right;
				}
			}
			else
				stack[sp++] = node->node.left;
		}
		else if (hitr && tnearr < out->t)
			stack[sp++] = node->node.right;
	}
	return (0);
}

int	scene_is_occluded(const t_scene *scene, const t_ray *ray,
				float max_distance, t_hit *hit)
{
	if (scene_is_occluded_planes(scene, ray, max_distance, hit))
		return (1);
	t_hit	out_hit = {};
	out_hit.t = max_distance;
	out_hit.primitive_id = hit->primitive_id;
	out_hit.kind = hit->kind;
	int status = bvh_ocult(*ray, scene->bvh_nodes, scene->primitives, &out_hit);
	return (status);
	// if (scene_is_occluded_primitives(scene, ray, max_distance))
	// 	return (0);
	return (0);
}
