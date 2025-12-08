/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_is_occluded.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 16:53:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/08 13:58:43 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"


static int	scene_is_occluded_planes(const t_scene *scene,
			const t_ray *ray, float max_distance)
{
	int		plane_index;
	float	hit_distance;

	plane_index = 0;
	while (plane_index < scene->plane_count)
	{
		if (inter_plane(&scene->planes[plane_index].pl,
				ray, &hit_distance))
		{
			if (hit_distance > 0.001f && hit_distance < max_distance)
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

int	scene_is_occluded(const t_scene *scene,
			const t_ray *ray, float max_distance)
{
	if (scene_is_occluded_planes(scene, ray, max_distance))
		return (1);
	if (scene_is_occluded_primitives(scene, ray, max_distance))
		return (1);
	return (0);
}
