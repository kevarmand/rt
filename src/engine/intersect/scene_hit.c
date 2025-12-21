/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_hit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:25:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 06:15:11 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include "bvh.h"
#include <math.h>

#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

static void	hit_try_update(t_hit *hit, t_hit_kind kind,
			int primitive_index, float distance)
{
	if (distance <= 0.0f)
		return ;
	if (distance >= hit->t)
		return ;
	hit->t = distance;
	hit->primitive_id = primitive_index;
	hit->kind = kind;
}

static void	scene_hit_prims(const t_scene *scene,
			const t_ray *ray, t_hit *out_hit)
{
	int	status;

	if (scene->bvh_node_count == 0)
		return ;
	status = bvh_inter(*ray, scene->bvh_nodes, scene->primitives, out_hit);
	if (status != 0)
		out_hit->kind = HIT_PRIMITIVE;
}

int	inter_plane(const t_plane *plane, const t_ray *ray, float *out_distance)
{
	float	denom;
	float	numerator;
	float	t;

	denom = vec3f_dot(plane->normal, ray->dir);
	if (fabsf(denom) < 1.0e-4f)
		return (0);
	numerator = - (vec3f_dot(plane->normal, ray->origin) + plane->d);
	t = numerator / denom;
	if (t <= 0.0f)
		return (0);
	*out_distance = t;
	return (1);
}

static void	scene_hit_planes(const t_scene *scene,
			const t_ray *ray, t_hit *out_hit)
{
	int		plane_index;
	float	hit_distance;

	plane_index = 0;
	while (plane_index < scene->plane_count)
	{
		if (inter_plane(&scene->planes[plane_index].pl,
				ray, &hit_distance))
			hit_try_update(out_hit, HIT_PLANE,
				plane_index, hit_distance);
		plane_index++;
	}
}

int	scene_hit(const t_scene *scene, const t_ray *ray, t_hit *out_hit)
{
	scene_hit_planes(scene, ray, out_hit);
	scene_hit_prims(scene, ray, out_hit);
	if (out_hit->kind == HIT_NONE)
		return (0);
	return (1);
}
