/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cam_view.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 15:30:33 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 15:32:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "scene.h"
#include <math.h>
#define M_PI 3.14159f

static void	set_pixel_step(t_cam_view *cv, float half_w, float half_h)
{
	cv->dx = vec3f_scale(cv->right, 2.0f * half_w);
	cv->dy = vec3f_scale(cv->up, -2.0f * half_h);
}

static void	set_first_pixel(t_cam_view *cv, float half_w, float half_h)
{
	t_vec3f tmp1;
	t_vec3f tmp2;

	tmp1 = vec3f_scale(cv->right, -half_w);
	tmp2 = vec3f_scale(cv->up, half_h);
	cv->p0 = vec3f_add(cv->origin, vec3f_add(tmp1, tmp2));
}

int	init_cam_views(t_engine *engine, const t_scene *scene)
{
	int			i;
	t_cam_view	*cv;
	t_camera	*src;
	float		half_w;
	float		half_h;

	engine->cam_count = scene->camera_count;
	engine->cam_views = malloc(sizeof(t_cam_view) * engine->cam_count);
	if (!engine->cam_views)
		return (ERR_MALLOC);
	i = 0;
	while (i < engine->cam_count)
	{
		cv = &engine->cam_views[i];
		src = &scene->cameras[i];
		cv->src = src;
		cv->origin = src->origin;
		cv->forward = src->forward;
		cv->right = src->right;
		cv->up = src->up;
		half_w = tanf((src->fov_deg * (float)M_PI / 180.0f) * 0.5f);
		half_h = half_w / src->aspect;
		set_first_pixel(cv, half_w, half_h);
		set_pixel_step(cv, half_w / engine->width, half_h / engine->height);
		i++;
	}
	return (SUCCESS);
}
