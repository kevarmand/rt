/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_refraction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 14:17:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 19:01:39 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "rt_config.h"
#include <math.h>

typedef struct s_refr_data
{
	t_vec3f	in_dir;
	t_vec3f	normal;
	float	ratio;
}	t_refr_data;

static void	refr_prepare(const t_hit *hit, const t_material *material,
				const t_shading_ctx *ctx, t_refr_data *data)
{
	float	n1;
	float	n2;

	data->in_dir = vec3f_scale(hit->view_dir, -1.0f);
	data->normal = hit->normal;
	if (vec3f_dot(data->normal, data->in_dir) > 0.0f)
		data->normal = vec3f_scale(data->normal, -1.0f);
	n1 = ctx->current_ior;
	if (n1 == 1.0f)
		n2 = material->ior;
	else
		n2 = 1.0f;
	data->ratio = n1 / n2;
}

static int	refr_compute_dir(t_vec3f in_dir, t_vec3f normal,
				float ratio, t_vec3f *dir_out)
{
	float	cos_i;
	float	sin2_t;

	cos_i = -vec3f_dot(normal, in_dir);
	sin2_t = ratio * ratio * (1.0f - cos_i * cos_i);
	if (sin2_t > 1.0f)
		return (0);
	*dir_out = vec3f_add(vec3f_scale(in_dir, ratio),
			vec3f_scale(normal, ratio * cos_i - sqrtf(1.0f - sin2_t)));
	return (1);
}

static int	build_refraction_ray(const t_hit *hit,
				const t_material *material,
				const t_shading_ctx *ctx,
				t_ray *refr_ray)
{
	t_refr_data	data;
	t_vec3f		dir;
	t_vec3f		dir_norm;

	refr_prepare(hit, material, ctx, &data);
	if (refr_compute_dir(data.in_dir, data.normal, data.ratio, &dir) == 0)
		return (0);
	dir_norm = vec3f_normalize(dir);
	*refr_ray = ray_finalize(vec3f_add(hit->point,
				vec3f_scale(dir_norm, 0.01f)), dir_norm);
	return (1);
}

t_vec3f	shade_refraction(const t_scene *scene,
			const t_hit *hit,
			t_shading_ctx *ctx,
			float factor)
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
