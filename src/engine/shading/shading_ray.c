/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_ray.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 21:10:33 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/09 11:54:43 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <math.h>

static void	reset_hit(t_hit *hit)
{
	hit->kind = HIT_NONE;
	hit->t = INFINITY;
	hit->point = (t_vec3f){0.0f, 0.0f, 0.0f, 0.0f};
	hit->normal = (t_vec3f){0.0f, 0.0f, 0.0f, 0.0f};
	hit->primitive_id = -1;
}

static float	sky_height(const t_vec3f direction)
{
	if (direction[1] < 0.0f)
		return (-direction[1]);
	return (direction[1]);
}

static void	sky_mix(const t_vec3f color_from, const t_vec3f color_to,
				float factor, t_vec3f *color_out)
{
	float	inverse;

	inverse = 1.0f - factor;
	(*color_out)[0] = color_from[0] * inverse + color_to[0] * factor;
	(*color_out)[1] = color_from[1] * inverse + color_to[1] * factor;
	(*color_out)[2] = color_from[2] * inverse + color_to[2] * factor;
}

void	sky_color(const t_vec3f direction, t_vec3f *color_out)
{
	const t_vec3f	color_night = {0.01f, 0.02f, 0.06f};
	const t_vec3f	color_blue = {0.40f, 0.70f, 1.00f};
	const t_vec3f	color_sunset = {1.00f, 0.55f, 0.40f};
	float			height;
	float			factor;

	height = sky_height(direction);
	if (height < 0.6f)
	{
		factor = height / 0.6f;
		sky_mix(color_sunset, color_blue, factor, color_out);
	}
	else
	{
		factor = (height - 0.6f) / 0.4f;
		sky_mix(color_blue, color_night, factor, color_out);
	}
}


int shading_ray(const t_scene *scene, const t_ray *ray, 
	t_shading_ctx *ctx, t_vec3f *color_out)
{
	t_hit	hit;

	reset_hit(&hit);
	if (scene_hit(scene, ray, &hit))
	{
		// hit.surface_id = scene->primitives[hit.primitive_id].surface_id;
		// *color_out = scene->surfaces[hit.surface_id].color;
		hit_build_geometry(scene, ray, &hit);
		apply_surface_shading(scene, &hit);
		shade_hit(scene, &hit, ctx, color_out);//peut etre regarder lerreur si un jour y en a une 
	}
	else
	{
		sky_color(ray->dir, color_out);
	}
	return (0);
}
