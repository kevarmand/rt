/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_hit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:25:56 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/30 13:10:12 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <stdio.h>


// static void	hit_try_update(t_hit *hit, t_hit_kind kind,
// 			int primitive_index, float distance)
// {
// 	if (distance <= 0.0f)
// 		return ;
// 	if (distance >= hit->t)
// 		return ;
// 	hit->t = distance;
// 	hit->primitive_id = primitive_index;
// 	hit->kind = kind;
// }

#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

static void	hit_try_update(t_hit *hit, t_hit_kind kind,
			int primitive_index, float distance)
{

	if (distance <= 0.0f)
	{
		return ;
	}
	if (distance >= hit->t)
	{
		return ;
	}


	hit->t = distance;
	hit->primitive_id = primitive_index;
	hit->kind = kind;
}


//dispatcher d'intersection selon le type de primitive
int	inter_primitive(const t_primitive *primitive,
			const t_ray *ray, float *out_distance)
{
	if (primitive->type == PRIM_SPHERE)
		return (inter_sphere(&primitive->sp, ray, out_distance));
	else if (primitive->type == PRIM_CYLINDER)
		return (inter_cylinder(&primitive->cy, ray, out_distance));
	else if (primitive->type == PRIM_TRIANGLE)
		return (inter_triangle(&primitive->tr, ray, out_distance));
	else if (primitive->type == PRIM_TORUS)
		return (inter_torus(&primitive->to, ray, out_distance));
	return (0);
}

static void	scene_hit_primitives(const t_scene *scene,
			const t_ray *ray, t_hit *out_hit)
{
	int		primitive_index;
	float	hit_distance;

	primitive_index = 0;
	while (primitive_index < scene->primitive_count)
	{
		if (inter_primitive(&scene->primitives[primitive_index],
				ray, &hit_distance))
			hit_try_update(out_hit, HIT_PRIMITIVE,
				primitive_index, hit_distance);
		primitive_index++;
	}
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
	scene_hit_primitives(scene, ray, out_hit);
	if (out_hit->kind == HIT_NONE)
		return (0);
	return (1);
}
