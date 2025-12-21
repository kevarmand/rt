/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_is_occluded.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:53:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 01:21:19 by norivier         ###   ########.fr       */
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

// static int	scene_is_occluded_primitives(const t_scene *scene,
// 			const t_ray *ray, float max_distance)
// {
// 	int		primitive_index;
// 	float	hit_distance;

// 	primitive_index = 0;
// 	while (primitive_index < scene->primitive_count)
// 	{
// 		if (inter_primitive(&scene->primitives[primitive_index],
// 				ray, &hit_distance))
// 		{
// 			if (hit_distance > 0.01f && hit_distance < max_distance)
// 				return (1);
// 		}
// 		primitive_index++;
// 	}
// 	return (0);
// }

int	bvh_ocult(t_ray r, t_bvhnode *nodes, t_primitive *prims, t_hit *out)
{
	uint32_t	stack[64];
	int			sp;

	sp = 0;
	stack[sp++] = 0;
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
				if (prim_inter(r, &prims[prim_id], &local_hit, tnear) != 0)
				{
					if (local_hit.t > 0.0f && local_hit.t < out->t)
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
	t_hit	local_hit;
	if (scene_is_occluded_planes(scene, ray, max_distance, hit))
		return (1);
	local_hit = *hit;
	local_hit.t = max_distance;
	return (bvh_ocult(*ray, scene->bvh_nodes, scene->primitives, &local_hit));
}
