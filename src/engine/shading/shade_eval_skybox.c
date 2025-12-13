/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade_eval_skybox.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 23:44:01 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/13 23:53:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "engine.h"
#include "scene.h"
#include "vector.h"


t_vec3f	sample_texture(const t_texture *t, float u, float v);

typedef enum e_skybox_mode
{
	SKYBOX_SPHERE = 0,
	SKYBOX_CYLINDER = 1
}	t_skybox_mode;

t_vec3f	vec3f_lerp(t_vec3f a, t_vec3f b, float t)
{
	t_vec3f	out;

	out = vec3f_add(vec3f_scale(a, 1.0f - t), vec3f_scale(b, t));
	return (out);
}

static float	clampf(float value, float min_value, float max_value)
{
	if (value < min_value)
		return (min_value);
	if (value > max_value)
		return (max_value);
	return (value);
}

static float	smoothstep(float edge0, float edge1, float value)
{
	float	t;

	t = (value - edge0) / (edge1 - edge0);
	t = clampf(t, 0.0f, 1.0f);
	return (t * t * (3.0f - 2.0f * t));
}

static void	sky_gradient_eval(t_vec3f dir, t_vec3f *out)
{
	t_vec3f	color_top;
	t_vec3f	color_mid;
	t_vec3f	color_bot;
	float	height;
	float	k;

	color_top = (t_vec3f){0.01f, 0.02f, 0.05f};
	color_mid = (t_vec3f){0.08f, 0.12f, 0.20f};
	color_bot = (t_vec3f){0.55f, 0.35f, 0.25f};
	height = clampf(dir[1] * 0.5f + 0.5f, 0.0f, 1.0f);
	if (height < 0.55f)
	{
		k = height / 0.55f;
		*out = vec3f_lerp(color_bot, color_mid, k);
		return ;
	}
	k = (height - 0.55f) / 0.45f;
	*out = vec3f_lerp(color_mid, color_top, k);
}

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

static void	sky_cylinder_eval(const t_scene *scene, t_vec3f dir, t_vec3f *out)
{
	const t_texture	*texture;
	t_vec3f			horizon;
	t_vec3f			grad;
	float			angle_deg;
	float			y_limit;
	float			y_abs;
	float			u;
	float			v;
	float			blend;

	angle_deg = 30.0f;
	y_limit = sinf(angle_deg * (float)M_PI / 180.0f);
	y_abs = fabsf(dir[1]);
	sky_gradient_eval(dir, &grad);
	if (y_abs >= y_limit)
	{
		*out = grad;
		return ;
	}
	texture = &scene->textures[scene->skybox.texture_id];
	sky_cyl_uv(dir, y_limit, &u, &v);
	horizon = sample_texture(texture, u, v);
	blend = smoothstep(y_limit * 0.70f, y_limit, y_abs);
	*out = vec3f_lerp(horizon, grad, blend);
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
