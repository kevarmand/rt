/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:13:26 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 23:50:47 by kearmand         ###   ########.fr       */
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
	t_vec3f	term3;
	t_vec3f	cross_axis;
	float	cos_angle;
	float	sin_angle;
	float	dot_axis_vector;

	axis = vec3f_normalize(axis);
	cos_angle = cosf(angle);
	sin_angle = sinf(angle);
	term1 = vec3f_scale(vector, cos_angle);
	cross_axis = vec3f_cross(axis, vector);
	term2 = vec3f_scale(cross_axis, sin_angle);
	dot_axis_vector = vec3f_dot(axis, vector);
	term3 = vec3f_scale(axis, dot_axis_vector * (1.0f - cos_angle));
	return (vec3f_add(vec3f_add(term1, term2), term3));
}

void	camera_apply_mouse_delta(t_scene *scene,
			t_display *display, int delta_x, int delta_y)
{
	t_camera	*camera;
	t_vec3f		up;
	float		yaw;
	float		pitch;
	float		aspect;
	float		fov_h;
	float		fov_v;

	camera = &scene->cameras[display->current_cam];
	if (delta_x == 0 && delta_y == 0)
		return ;
	aspect = (float)scene->resolution_width
		/ (float)scene->resolution_height;
	fov_h = camera->fov_deg * (float)M_PI / 180.0f;
	fov_v = 2.0f * atanf(tanf(fov_h * 0.5f) / aspect);
	yaw = ((float)delta_x / (float)scene->resolution_width) * fov_h;
	pitch = ((float)delta_y / (float)scene->resolution_height) * fov_v;
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
			t_display *display, int delta_x)
{
	t_camera	*camera;
	float		angle;

	camera = &scene->cameras[display->current_cam];
	if (delta_x == 0)
		return ;
	angle = (float)delta_x * 0.0025f;
	camera->right = vec3f_rotate_axis(camera->right,
			camera->forward, angle);
	camera->right = vec3f_normalize(camera->right);
}


static int	camera_handle_drag(t_data *data)
{
	t_display		*display;
	t_mouse_state	*mouse;
	int				delta_x;
	int				delta_y;

	display = &data->display;
	mouse = &display->mouse;
	if (mouse->mode == MOUSE_MODE_NONE)
		return (0);
	delta_x = mouse->accum_dx;
	delta_y = mouse->accum_dy;
	if (delta_x == 0 && delta_y == 0)
		return (0);
	mouse->accum_dx = 0;
	mouse->accum_dy = 0;
	if (mouse->mode == MOUSE_MODE_CAM_FREE)
		camera_apply_mouse_delta(&data->scene, display, delta_x, delta_y);
	else if (mouse->mode == MOUSE_MODE_CAM_ROLL)
		camera_apply_roll(&data->scene, display, delta_x);
	return (1);
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
	if (new_right[0] == 0.0f && new_right[1] == 0.0f && new_right[2] == 0.0f)
		return ;
	camera->right = vec3f_normalize(new_right);
}
#include <stdio.h>
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
		printf("icing\n");
		camera_level(&data->scene, display);
		display->flag_camera_level = 0;
		changed = 1;
	}
	if (!changed)
		return ;
	printf("Camera updated via mouse input.\n");
	display->frame[display->current_cam].is_dirty = 1;
	display->flag_camera_changed = 1;
}
