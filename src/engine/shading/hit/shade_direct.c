/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_direct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:04:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 19:04:12 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "rt_config.h"
#include "scene.h"
#include <math.h>

typedef struct s_ctx_light
{
	const t_material	*material;
	const t_surface_map	*surface;
	const t_light		*light;
	t_vec3f				point;
	t_vec3f				normal;
	t_vec3f				view_dir;
	t_vec3f				light_dir;
	float				light_dist;
	t_vec3f				albedo;
}	t_ctx_light;

static void	init_ctx_light(const t_scene *scene,
			const t_hit *hit,
			int light_index,
			t_ctx_light *ctx)
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
	t_hit		hit_tmp;

	init_ctx_light(scene, hit, light_i, &ctx);
	if (ctx.light_dist <= 0.0f)
		return ;
	if (hit->kind == HIT_PLANE)
		offset = 0.0f;
	else
		offset = vec3f_scale(ctx.normal, TMIN_SHADOW);
	shadow_ray.origin = vec3f_add(ctx.point, offset);
	shadow_ray = ray_finalize(shadow_ray.origin, ctx.light_dir);
	hit_tmp = *hit;
	if (scene_is_occluded(scene, &shadow_ray, ctx.light_dist, &hit_tmp))
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
