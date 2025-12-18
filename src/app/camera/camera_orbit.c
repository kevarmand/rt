/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_orbit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:47:20 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 18:56:45 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

typedef struct s_orbit_ctx
{
	t_camera	*cam;
	t_cam_ctrl	*ctrl;
	t_vec3f		world_up;
	t_vec3f		offset;
	float		yaw;
	float		pitch;
}	t_orbit_ctx;

static float	camera_orbit_yaw(const t_scene *scene, int delta_x)
{
	float	width_f;
	float	delta_f;

	width_f = (float)scene->resolution_width;
	delta_f = (float)delta_x;
	return (delta_f / width_f * (2.0f * (float)M_PI));
}

static float	camera_orbit_pitch(const t_scene *scene, int delta_y)
{
	float	height_f;
	float	delta_f;

	height_f = (float)scene->resolution_height;
	delta_f = (float)delta_y;
	return (delta_f / height_f * (float)M_PI);
}

static void	camera_apply_orbit(t_scene *scene, t_display *display,
				int delta_x, int delta_y)
{
	t_orbit_ctx	ctx;

	ctx.cam = &scene->cameras[display->current_cam];
	ctx.ctrl = &display->cam_ctrl;
	ctx.world_up = (t_vec3f){0.0f, -1.0f, 0.0f};
	ctx.yaw = camera_orbit_yaw(scene, -delta_x);
	ctx.pitch = camera_orbit_pitch(scene, delta_y);
	ctx.offset = vec3f_sub(ctx.cam->origin, ctx.ctrl->center);
	if (ctx.yaw != 0.0f)
	{
		ctx.offset = vec3f_rot_axis(ctx.offset, ctx.world_up, -ctx.yaw);
		ctx.cam->forward = vec3f_rot_axis(ctx.cam->forward, ctx.world_up,
				-ctx.yaw);
		ctx.cam->right = vec3f_rot_axis(ctx.cam->right, ctx.world_up, -ctx.yaw);
	}
	ctx.cam->right = vec3f_normalize(ctx.cam->right);
	ctx.cam->forward = vec3f_normalize(ctx.cam->forward);
	if (ctx.pitch != 0.0f)
	{
		ctx.offset = vec3f_rot_axis(ctx.offset, ctx.cam->right, -ctx.pitch);
		ctx.cam->forward = vec3f_rot_axis(ctx.cam->forward,
				ctx.cam->right, -ctx.pitch);
		ctx.cam->forward = vec3f_normalize(ctx.cam->forward);
	}
	ctx.cam->origin = vec3f_add(ctx.ctrl->center, ctx.offset);
}

int	camera_handle_orbit(t_data *data)
{
	t_display		*display;
	t_mouse_state	*mouse;
	int				delta_x;
	int				delta_y;

	display = &data->display;
	mouse = &display->mouse;
	if (mouse->is_down == 0 || mouse->button != MOUSE_BUTTON_LEFT)
		return (0);
	delta_x = mouse->current_x - mouse->anchor_x;
	delta_y = mouse->current_y - mouse->anchor_y;
	if (delta_x == 0 && delta_y == 0)
		return (0);
	mouse->anchor_x = mouse->current_x;
	mouse->anchor_y = mouse->current_y;
	camera_apply_orbit(&data->scene, display, delta_x, delta_y);
	return (1);
}
