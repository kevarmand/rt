/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox_eval.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 23:44:01 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/18 18:43:03 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "scene.h"
#include "vector.h"
#include "../shading.h"

void	sky_gradient_eval(t_vec3f dir, t_vec3f *out);
float	clampf(float value, float min_value, float max_value);
float	smoothstep(float edge0, float edge1, float value);

typedef enum e_skybox_mode
{
	SKYBOX_SPHERE = 0,
	SKYBOX_CYLINDER = 1
}	t_skybox_mode;

static void	sky_sphere_uv(t_vec3f dir, float *u, float *v)
{
	float	phi;
	float	theta;

	phi = atan2f(dir[2], dir[0]);
	theta = asinf(clampf(dir[1], -1.0f, 1.0f));
	*u = (phi / (2.0f * (float)M_PI)) + 0.5f;
	*v = (theta / (float)M_PI) + 0.5f;
}

static void	sky_sphere_eval(const t_scene *scene, t_vec3f dir, t_vec3f *out)
{
	const t_texture	*texture;
	float			u;
	float			v;

	texture = &scene->textures[scene->skybox.texture_id];
	sky_sphere_uv(dir, &u, &v);
	*out = sample_texture(texture, u, v);
}

static void	sky_cyl_uv(t_vec3f dir, float y_limit, float *u, float *v)
{
	float	phi;
	float	y;

	phi = atan2f(dir[2], dir[0]);
	*u = (phi / (2.0f * (float)M_PI)) + 0.5f;
	y = clampf(dir[1], -y_limit, y_limit);
	*v = (y / (2.0f * y_limit)) + 0.5f;
}

typedef struct s_ctx_sky_cyl
{
	float			y_limit;
	float			y_abs;
	float			u;
	float			v;
	float			blend;
}	t_ctx_sky_cyl;

static void	sky_cylinder_eval(const t_scene *scene, t_vec3f dir, t_vec3f *out)
{
	const t_texture	*texture;
	t_vec3f			horizon;
	t_vec3f			grad;
	t_ctx_sky_cyl	ctx;

	ctx.y_limit = sinf(30.0f * (float)M_PI / 180.0f);
	ctx.y_abs = fabsf(dir[1]);
	sky_gradient_eval(dir, &grad);
	if (ctx.y_abs >= ctx.y_limit)
	{
		*out = grad;
		return ;
	}
	texture = &scene->textures[scene->skybox.texture_id];
	sky_cyl_uv(dir, ctx.y_limit, &ctx.u, &ctx.v);
	horizon = sample_texture(texture, ctx.u, ctx.v);
	ctx.blend = smoothstep(ctx.y_limit * 0.70f, ctx.y_limit, ctx.y_abs);
	*out = vec3f_lerp(horizon, grad, ctx.blend);
}

void	skybox_eval(t_vec3f dir, const t_scene *scene, t_vec3f *color_out)
{
	if (scene->skybox.texture_id == -1)
	{
		sky_gradient_eval(dir, color_out);
		return ;
	}
	if (scene->skybox.mode == SKYBOX_CYLINDER)
	{
		sky_cylinder_eval(scene, dir, color_out);
		return ;
	}
	sky_sphere_eval(scene, dir, color_out);
}
