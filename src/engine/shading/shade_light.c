/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:04:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 22:05:04 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "rt_config.h"
#include "scene.h"

// static void	shade_one_light(const t_scene *scene,
// 			const t_hit *hit, int light_index, t_vec3f *color)
// {
// 	const t_primitive	*primitive;
// 	const t_material	*material;
// 	const t_surface		*surface;
// 	const t_light		*light;
// 	t_vec3f				light_vec;
// 	t_vec3f				light_dir;
// 	t_vec3f				offset_origin;
// 	t_ray				shadow_ray;
// 	float				light_dist;
// 	float				ndotl;
// 	t_vec3f				tmp;

// 	primitive = &scene->primitives[hit->primitive_id];
// 	material = &scene->materials[hit->material_id];
// 	surface = &scene->surfaces[hit->surface_id];
// 	light = &scene->lights[light_index];
// 	light_vec = vec3f_sub(light->position, hit->point);
// 	light_dist = vec3f_length(light_vec);
// 	if (light_dist <= 0.0f)
// 		return ;
// 	light_dir = vec3f_scale(light_vec, 1.0f / light_dist);
// 	offset_origin = vec3f_add(hit->point,
// 			vec3f_scale(hit->normal, 1e-4f));
// 	shadow_ray.origin = offset_origin;
// 	shadow_ray.dir = light_dir;
// 	if (scene_is_occluded(scene, &shadow_ray, light_dist))
// 		return ;
// 	ndotl = vec3f_dot(hit->normal, light_dir);
// 	if (ndotl <= 0.0001f)
// 		return ;
// 	tmp = vec3f_mul(surface->color, light->color);
// 	tmp = vec3f_scale(tmp, material->diffuse
// 	 		* light->intensity * ndotl);
// 	*color = vec3f_add(*color, tmp);
// }


// void	shade_direct_lights(const t_scene *scene,
// 			const t_hit *hit, t_vec3f *color)
// {
// 	int	light_index;

// 	light_index = 0;
// 	while (light_index < scene->light_count)
// 	{
// 		shade_one_light(scene, hit, light_index, color);
// 		light_index++;
// 	}
// }

#include <math.h>

typedef struct s_ctx_light
{
	const t_material	*material;
	const t_surface		*surface;
	const t_light		*light;
	t_vec3f				point;
	t_vec3f				normal;
	t_vec3f				view_dir;
	t_vec3f				light_dir;
	float				light_dist;
	t_vec3f				albedo;
}	t_ctx_light;

static void	init_ctx_light(const t_scene *scene,
			const t_hit *hit, int light_index, t_ctx_light *ctx)
{
	const t_primitive	*prim;
	t_vec3f				tmp;

	prim = &scene->primitives[hit->primitive_id];
	ctx->material = &scene->materials[hit->material_id];
	ctx->surface = &scene->surfaces[hit->surface_id];
	ctx->light = &scene->lights[light_index];
	ctx->albedo = hit->albedo;
	ctx->point = hit->point;
	ctx->normal = hit->normal;
	ctx->view_dir = hit->view_dir;
	tmp = vec3f_sub(ctx->light->position, ctx->point);
	ctx->light_dist = vec3f_length(tmp);
	if (ctx->light_dist > 0.0f)
		ctx->light_dir = vec3f_scale(tmp, 1.0f / ctx->light_dist);
	else
		ctx->light_dir = (t_vec3f){0.0f, 0.0f, 0.0f};
	(void)prim;
}

static void	shade_diffuse(const t_ctx_light *ctx, t_vec3f *color)
{
	float	ndotl;
	t_vec3f	tmp;

	ndotl = vec3f_dot(ctx->normal, ctx->light_dir);
	if (ndotl <= 0.0f)
		return ;
	tmp = vec3f_mul(ctx->albedo, ctx->light->color);
	tmp = vec3f_scale(tmp,
			ctx->material->diffuse
			* ctx->light->intensity
			* ndotl);
	*color = vec3f_add(*color, tmp);
}

static void	shade_specular_blinn(const t_ctx_light *ctx, t_vec3f *color)
{
	t_vec3f	h;
	float	ndoth;
	float	spec;
	t_vec3f	tmp;

	if (ctx->material->specular <= 0.0f)
		return ;
	h = vec3f_add(ctx->light_dir, ctx->view_dir);
	h = vec3f_normalize(h);
	ndoth = vec3f_dot(ctx->normal, h);
	if (ndoth <= 0.0f)
		return ;
	spec = powf(ndoth, ctx->material->shininess);
	spec *= ctx->material->specular * ctx->light->intensity;
	tmp = vec3f_scale(ctx->light->color, spec);
	*color = vec3f_add(*color, tmp);
}

void	shade_one_light(const t_scene *scene,
			const t_hit *hit, int light_i, t_vec3f *color)
{
	t_ctx_light	ctx;
	t_ray		shadow_ray;
	t_vec3f		offset;

	init_ctx_light(scene, hit, light_i, &ctx);
	if (ctx.light_dist <= 0.0f)
		return ;
	if (hit->kind == HIT_PLANE)
		offset = 0.0f;
	else
		offset = vec3f_scale(ctx.normal, fmaxf(TMIN_SHADOW, TMIN_SHADOW_BIAS * hit->t));
	shadow_ray.origin = vec3f_add(ctx.point, offset);
	// shadow_ray.origin = vec3f_add(shadow_ray.origin,
	// 		vec3f_scale(ctx.light_dir, 0.0f));
	shadow_ray = build_ray(shadow_ray.origin, ctx.light_dir);
	if (scene_is_occluded(scene, &shadow_ray, ctx.light_dist, hit))
		return ;
	shade_diffuse(&ctx, color);
	shade_specular_blinn(&ctx, color);
}

void	shade_direct_lights(const t_scene *scene,
			const t_hit *hit, t_vec3f *color)
{
	int	i;

	i = 0;
	while (i < scene->light_count)
	{
		shade_one_light(scene, hit, i, color);
		i++;
	}
}
