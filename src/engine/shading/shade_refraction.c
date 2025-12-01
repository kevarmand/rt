/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_refraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 14:17:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/30 18:27:15 by kearmand         ###   ########.fr       */
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
	t_vec3f	term_parallel;
	t_vec3f	term_perp;
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
	term_parallel = vec3f_scale(in_dir, ratio);
	term_perp = vec3f_scale(normal, ratio * cos_i - cos_t);
	dir = vec3f_add(term_parallel, term_perp);
	refr_ray->dir = vec3f_normalize(dir);
	refr_ray->origin = vec3f_add(hit->point,
			vec3f_scale(refr_ray->dir, 1e-2f));
	return (1);
}


void	shade_refraction(const t_scene *scene,
			const t_hit *hit, t_shading_ctx *ctx, t_vec3f *color)
{
	const t_material	*material;
	float				kt;
	t_shading_ctx		child;
	t_ray				refr_ray;
	t_vec3f				refr_color;

	material = &scene->materials[hit->material_id];
	kt = material->refraction;
	if (kt <= 0.0f)
		return ;
	if (ctx->depth >= MAX_RECURSION_DEPTH)
		return ;
	if (!build_refraction_ray(hit, material, ctx, &refr_ray))
		return ;
	child = *ctx;
	child.depth++;
	if (ctx->current_ior == 1.0f)
		child.current_ior = material->ior;
	else
		child.current_ior = 1.0f;
	shading_ray(scene, &refr_ray, &child, &refr_color);
	*color = vec3f_add(
			vec3f_scale(*color, 1.0f - kt),
			vec3f_scale(refr_color, kt));
}

