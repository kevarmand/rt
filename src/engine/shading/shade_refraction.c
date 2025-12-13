/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_refraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 14:17:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/13 21:00:33 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "rt_config.h"
#include <math.h>

static int	build_refraction_ray(const t_hit *hit,
				const t_material *material,
				const t_shading_ctx *ctx,
				t_ray *refr_ray)
{
	t_vec3f	in_dir;
	t_vec3f	normal;
	float	n1;
	float	n2;
	float	ratio;
	float	cos_i;
	float	sin2_t;
	float	cos_t;
	t_vec3f	dir;

	in_dir = vec3f_scale(hit->view_dir, -1.0f);
	normal = hit->normal;
	if (vec3f_dot(normal, in_dir) > 0.0f)
		normal = vec3f_scale(normal, -1.0f);
	n1 = ctx->current_ior;
	if (n1 == 1.0f)
		n2 = material->ior;
	else
		n2 = 1.0f;
	ratio = n1 / n2;
	cos_i = -vec3f_dot(normal, in_dir);
	sin2_t = ratio * ratio * (1.0f - cos_i * cos_i);
	if (sin2_t > 1.0f)
		return (0);
	cos_t = sqrtf(1.0f - sin2_t);
	dir = vec3f_add(vec3f_scale(in_dir, ratio),
			vec3f_scale(normal, ratio * cos_i - cos_t));
	*refr_ray = build_ray(vec3f_add(hit->point,
				vec3f_scale(vec3f_normalize(dir), 1e-2f)),
			vec3f_normalize(dir));
	return (1);
}

t_vec3f	shade_refraction(const t_scene *scene,
			const t_hit *hit, t_shading_ctx *ctx, float factor)
{
	const t_material	*material;
	t_shading_ctx		child;
	t_ray				refr_ray;
	t_vec3f				refr_color;

	child = *ctx;
	child.depth++;
	child.contribution = ctx->contribution * factor;
	if (child.depth >= MAX_RECURSION_DEPTH
		|| child.contribution < MIN_CONTRIBUTION)
		return ((t_vec3f){0.0f, 0.0f, 0.0f});
	material = &scene->materials[hit->material_id];
	if (!build_refraction_ray(hit, material, ctx, &refr_ray))
		return ((t_vec3f){0.0f, 0.0f, 0.0f});
	if (ctx->current_ior == 1.0f)
		child.current_ior = material->ior;
	else
		child.current_ior = 1.0f;
	shading_ray(scene, &refr_ray, &child, &refr_color);
	return (vec3f_scale(refr_color, factor));
}


