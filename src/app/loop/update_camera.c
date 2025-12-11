/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:13:26 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/09 17:21:21 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"
#include "vector.h"
#include <math.h>

static t_vec3f	vec3f_rotate_axis(t_vec3f vector,
					t_vec3f axis, float angle)
{
	t_vec3f	term1;
	t_vec3f	term2;
	float	cos_angle;
	float	sin_angle;
	float	dot_axis_vector;

	axis = vec3f_normalize(axis);
	cos_angle = cosf(angle);
	sin_angle = sinf(angle);
	term1 = vec3f_scale(vector, cos_angle);
	term2 = vec3f_cross(axis, vector);
	term2 = vec3f_scale(term2, sin_angle);
	term1 = vec3f_add(term1, term2);
	dot_axis_vector = vec3f_dot(axis, vector);
	term2 = vec3f_scale(axis,
			dot_axis_vector * (1.0f - cos_angle));
	return (vec3f_add(term1, term2));
}

static float	camera_mouse_yaw(const t_scene *scene,
					const t_camera *camera, int delta_x)
{
	float	fov_h;
	float	width_f;
	float	delta_f;

	fov_h = camera->fov_deg * (float)M_PI / 180.0f;
	width_f = (float)scene->resolution_width;
	delta_f = (float)delta_x;
	return (delta_f / width_f * fov_h);
}

static float	camera_mouse_pitch(const t_scene *scene,
					const t_camera *camera, int delta_y)
{
	float	aspect;
	float	fov_h;
	float	fov_v;
	float	height_f;
	float	delta_f;

	aspect = (float)scene->resolution_width
		/ (float)scene->resolution_height;
	fov_h = camera->fov_deg * (float)M_PI / 180.0f;
	fov_v = 2.0f * atanf(tanf(fov_h * 0.5f) / aspect);
	height_f = (float)scene->resolution_height;
	delta_f = (float)delta_y;
	return (delta_f / height_f * fov_v);
}

void	camera_apply_mouse_delta(t_scene *scene,
			t_display *display, int delta_x, int delta_y)
{
	t_camera	*camera;
	t_vec3f		up;
	float		yaw;
	float		pitch;

	if (delta_x == 0 && delta_y == 0)
		return ;
	camera = &scene->cameras[display->current_cam];
	yaw = camera_mouse_yaw(scene, camera, delta_x);
	pitch = camera_mouse_pitch(scene, camera, delta_y);
	up = vec3f_cross(camera->right, camera->forward);
	up = vec3f_normalize(up);
	if (yaw != 0.0f)
	{
		camera->forward = vec3f_rotate_axis(camera->forward, up, yaw);
		camera->forward = vec3f_normalize(camera->forward);
		camera->right = vec3f_cross(camera->forward, up);
		camera->right = vec3f_normalize(camera->right);
	}
	if (pitch != 0.0f)
	{
		camera->forward = vec3f_rotate_axis(camera->forward,
				camera->right, pitch);
		camera->forward = vec3f_normalize(camera->forward);
	}
}

static void	camera_apply_roll(t_scene *scene,
			t_display *display, float angle)
{
	t_camera	*camera;

	if (angle == 0.0f)
		return ;
	camera = &scene->cameras[display->current_cam];
	camera->right = vec3f_rotate_axis(camera->right,
			camera->forward, angle);
	camera->right = vec3f_normalize(camera->right);
}

static float	camera_roll_angle(const t_data *data,
					const t_mouse_state *mouse)
{
	int		center_x;
	int		center_y;
	float	v0x;
	float	v0y;
	float	angle;

	center_x = data->scene.resolution_width / 2;
	center_y = data->scene.resolution_height / 2;
	v0x = (float)(mouse->anchor_x - center_x);
	v0y = (float)(mouse->anchor_y - center_y);
	if (v0x == 0.0f && v0y == 0.0f)
		return (0.0f);
	if ((float)(mouse->current_x - center_x) == 0.0f
		&& (float)(mouse->current_y - center_y) == 0.0f)
		return (0.0f);
	angle = atan2f(
			v0x * (float)(mouse->current_y - center_y)
			- v0y * (float)(mouse->current_x - center_x),
			v0x * (float)(mouse->current_x - center_x)
			+ v0y * (float)(mouse->current_y - center_y));
	return (angle);
}

static int	camera_handle_drag_free(t_data *data)
{
	t_mouse_state	*mouse;
	int				dx;
	int				dy;

	mouse = &data->display.mouse;
	dx = mouse->current_x - mouse->anchor_x;
	dy = mouse->current_y - mouse->anchor_y;
	if (dx == 0 && dy == 0)
		return (0);
	mouse->anchor_x = mouse->current_x;
	mouse->anchor_y = mouse->current_y;
	camera_apply_mouse_delta(&data->scene, &data->display, dx, dy);
	return (1);
}

static int	camera_handle_drag_roll(t_data *data)
{
	t_mouse_state	*mouse;
	float			angle;
	float			delta;

	mouse = &data->display.mouse;
	angle = camera_roll_angle(data, mouse);
	if (angle == 0.0f)
		return (0);
	delta = angle - mouse->roll_prev_angle;
	if (delta == 0.0f)
		return (0);
	mouse->roll_prev_angle = angle;
	camera_apply_roll(&data->scene, &data->display, -delta);
	return (1);
}

static int	camera_handle_drag(t_data *data)
{
	t_mouse_state	*mouse;

	mouse = &data->display.mouse;
	if (mouse->mode == MOUSE_MODE_CAM_FREE)
		return (camera_handle_drag_free(data));
	if (mouse->mode == MOUSE_MODE_CAM_ROLL)
		return (camera_handle_drag_roll(data));
	return (0);
}

static int	camera_handle_scroll(t_data *data)
{
	t_display		*display;
	t_mouse_state	*mouse;
	t_camera		*camera;
	int				scroll;
	float			step;

	display = &data->display;
	mouse = &display->mouse;
	scroll = mouse->scroll_delta;
	if (scroll == 0)
		return (0);
	camera = &data->scene.cameras[display->current_cam];
	step = 0.5f * (float)scroll;
	camera->origin = vec3f_add(camera->origin,
			vec3f_scale(camera->forward, step));
	mouse->scroll_delta = 0;
	return (1);
}

static void	camera_level(t_scene *scene, t_display *display)
{
	t_camera	*camera;
	t_vec3f		world_up;
	t_vec3f		new_right;

	camera = &scene->cameras[display->current_cam];
	world_up[0] = 0.0f;
	world_up[1] = -1.0f;
	world_up[2] = 0.0f;
	new_right = vec3f_cross(world_up, camera->forward);
	if (new_right[0] == 0.0f
		&& new_right[1] == 0.0f
		&& new_right[2] == 0.0f)
		return ;
	camera->right = vec3f_normalize(new_right);
}

void	display_update_camera(t_data *data)
{
	t_display	*display;
	int			changed;

	display = &data->display;
	changed = 0;
	changed |= camera_handle_drag(data);
	changed |= camera_handle_scroll(data);
	if (display->flag_camera_level)
	{
		camera_level(&data->scene, display);
		display->flag_camera_level = 0;
		changed = 1;
	}
	if (!changed)
		return ;
	display->flag_camera_changed = 1;
}
