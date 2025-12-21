/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_is_occluded.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:53:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 05:14:25 by kearmand         ###   ########.fr       */
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

int	scene_is_occluded(const t_scene *scene, const t_ray *ray,
				float max_distance, t_hit *hit)
{
	t_hit	local_hit;
	int		status;

	if (scene_is_occluded_planes(scene, ray, max_distance, hit))
		return (1);
	local_hit = *hit;
	local_hit.t = max_distance;
	status = 0;
	if (scene->bvh_node_count > 0)
		status = bvh_shadow(*ray, scene->bvh_nodes,
			scene->primitives, &local_hit);
	return (status);
}
