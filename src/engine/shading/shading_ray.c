/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_ray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:10:33 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 17:28:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>
#include "shading.h"

static void	reset_hit(t_hit *hit)
{
	hit->kind = HIT_NONE;
	hit->t = INFINITY;
	hit->point = (t_vec3f){0.0f, 0.0f, 0.0f, 0.0f};
	hit->normal = (t_vec3f){0.0f, 0.0f, 0.0f, 0.0f};
	hit->primitive_id = -1;
}

int	shading_ray(const t_scene *scene, const t_ray *ray,
	t_shading_ctx *ctx, t_vec3f *color_out)
{
	t_hit	hit;

	reset_hit(&hit);
	if (scene_hit(scene, ray, &hit))
	{
		hit_build_geometry(scene, ray, &hit);
		apply_surface_shading(scene, &hit);
		shade_hit(scene, &hit, ctx, color_out);
	}
	else
	{
		skybox_eval(ray->dir, scene, color_out);
	}
	return (0);
}

int	shading_ray_fast(const t_scene *scene, const t_ray *ray,
	t_shading_ctx *ctx, t_vec3f *color_out)
{
	t_hit	hit;

	(void)ctx;
	reset_hit(&hit);
	if (scene_hit(scene, ray, &hit))
	{
		if (hit.kind == HIT_PLANE)
			hit.surface_id = scene->planes[hit.primitive_id].surface_id;
		else if (hit.kind == HIT_PRIMITIVE)
			hit.surface_id = scene->primitives[hit.primitive_id].surface_id;
		*color_out = scene->surfaces[hit.surface_id].color;
	}
	else
		sky_color(ray->dir, color_out);
	return (0);
}
