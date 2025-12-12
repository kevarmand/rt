/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:13:26 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/12 16:31:27 by kearmand         ###   ########.fr       */
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


static void	camera_apply_roll(t_scene *scene, t_display *display, float angle)
{
	t_camera	*camera;

	if (angle == 0.0f)
		return ;
	camera = &scene->cameras[display->current_cam];
	camera->right = vec3f_rotate_axis(camera->right,
			camera->forward, angle);
	camera->right = vec3f_normalize(camera->right);
}
static float	camera_roll_angle(const t_data *data, const t_mouse_state *mouse)
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




void	camera_apply_mouse_delta(t_scene *scene,
			t_display *display, int delta_x, int delta_y)
{
	t_camera	*camera;
	t_vec3f		up;
	float		yaw;
	float		pitch;

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

static int	camera_handle_look(t_data *data)
{
	t_mouse_state	*mouse;
	int				delta_x;
	int				delta_y;

	mouse = &data->display.mouse;
	if (mouse->is_down == 0 || mouse->button != MOUSE_BUTTON_LEFT)
		return (0);
	delta_x = mouse->current_x - mouse->anchor_x;
	delta_y = mouse->current_y - mouse->anchor_y;
	if (delta_x == 0 && delta_y == 0)
		return (0);
	mouse->anchor_x = mouse->current_x;
	mouse->anchor_y = mouse->current_y;
	camera_apply_mouse_delta(&data->scene, &data->display, delta_x, delta_y);
	return (1);
}

static int	camera_handle_roll(t_data *data)
{
	t_display		*display;
	t_cam_ctrl		*ctrl;
	t_mouse_state	*mouse;
	float			angle;
	float			delta;

	display = &data->display;
	ctrl = &display->cam_ctrl;
	mouse = &display->mouse;
	if (mouse->is_down == 0 || mouse->button != MOUSE_BUTTON_LEFT)
		return (0);
	angle = camera_roll_angle(data, mouse);
	if (angle == 0.0f)
		return (0);
	if (ctrl->roll_prev_angle == 0.0f)
	{
		ctrl->roll_prev_angle = angle;
		return (0);
	}
	delta = angle - ctrl->roll_prev_angle;
	if (delta == 0.0f)
		return (0);
	ctrl->roll_prev_angle = angle;
	camera_apply_roll(&data->scene, display, delta);
	return (1);
}

static int	camera_handle_dolly(t_data *data)
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
	step = (float)scroll * display->cam_ctrl.nav_scale
		* display->cam_ctrl.nav_mul * 0.02f;
	camera->origin = vec3f_add(camera->origin,
			vec3f_scale(camera->forward, step));
	mouse->scroll_delta = 0;
	return (1);
}
static int	camera_handle_level(t_data *data)
{
	t_display	*display;
	t_camera	*camera;
	t_vec3f		world_up;
	t_vec3f		new_right;

	display = &data->display;
	if (!display->cam_ctrl.flag_level)
		return (0);
	camera = &data->scene.cameras[display->current_cam];
	world_up = (t_vec3f){0.0f, -1.0f, 0.0f};
	new_right = vec3f_cross(world_up, camera->forward);
	if (new_right[0] != 0.0f
		|| new_right[1] != 0.0f
		|| new_right[2] != 0.0f)
		camera->right = vec3f_normalize(new_right);
	display->cam_ctrl.flag_level = 0;
	return (1);
}


static int	camera_handle_pan(t_data *data)
{
	t_display		*display;
	t_mouse_state	*mouse;
	t_camera		*camera;
	t_vec3f			up;
	float			scale;
	float			step_x;
	float			step_y;
	int				delta_x;
	int				delta_y;

	display = &data->display;
	mouse = &display->mouse;
	if (mouse->is_down == 0 || mouse->button != MOUSE_BUTTON_RIGHT)
		return (0);
	delta_x = mouse->current_x - mouse->anchor_x;
	delta_y = mouse->current_y - mouse->anchor_y;
	if (delta_x == 0 && delta_y == 0)
		return (0);
	mouse->anchor_x = mouse->current_x;
	mouse->anchor_y = mouse->current_y;
	camera = &data->scene.cameras[display->current_cam];
	up = vec3f_cross(camera->right, camera->forward);
	up = vec3f_normalize(up);
	scale = display->cam_ctrl.nav_scale * display->cam_ctrl.nav_mul;
	step_x = (float)delta_x / (float)data->scene.resolution_width;
	step_y = (float)delta_y / (float)data->scene.resolution_height;
	camera->origin = vec3f_sub(camera->origin,
			vec3f_scale(camera->right, step_x * scale));
	camera->origin = vec3f_add(camera->origin,
			vec3f_scale(up, step_y * scale));
	return (1);
}


static void	camera_apply_orbit(t_scene *scene, t_display *display,
				int delta_x, int delta_y)
{
	t_camera	*camera;
	t_cam_ctrl	*ctrl;
	t_vec3f		world_up;
	t_vec3f		offset;
	t_vec3f		up;
	float		yaw;
	float		pitch;

	camera = &scene->cameras[display->current_cam];
	ctrl = &display->cam_ctrl;
	world_up[0] = 0.0f;
	world_up[1] = -1.0f;
	world_up[2] = 0.0f;
	yaw = camera_mouse_yaw(scene, camera, delta_x);
	pitch = camera_mouse_pitch(scene, camera, delta_y);
	offset = vec3f_sub(camera->origin, ctrl->center);
	if (yaw != 0.0f)
	{
		offset = vec3f_rotate_axis(offset, world_up, -yaw);
		camera->right = vec3f_rotate_axis(camera->right, world_up, -yaw);
	}
	if (pitch != 0.0f)
		offset = vec3f_rotate_axis(offset, camera->right, -pitch);
	camera->origin = vec3f_add(ctrl->center, offset);
	camera->forward = vec3f_normalize(vec3f_sub(ctrl->center, camera->origin));
	up = vec3f_normalize(vec3f_cross(camera->right, camera->forward));
	camera->right = vec3f_normalize(vec3f_cross(camera->forward, up));
}

static int	camera_handle_orbit(t_data *data)
{
	t_display		*display;
	t_mouse_state	*mouse;
	int				delta_x;
	int				delta_y;

	display = &data->display;
	mouse = &display->mouse;
	if (mouse->is_down == 0 || mouse->button != MOUSE_BUTTON_RIGHT)
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


void	display_update_camera(t_data *data)
{
	t_display	*display;
	int			changed;

	display = &data->display;
	changed = 0;
	if (display->cam_ctrl.mode == CAM_MODE_STANDARD)
	{
		changed |= camera_handle_look(data);//left en mode normal : rotation de la camera 
		changed |= camera_handle_pan(data);//right en mode normal : deplacement de la camera
	}
	else if (display->cam_ctrl.mode == CAM_MODE_ROLL)
	{
		changed |= camera_handle_roll(data);//left en mode roll : rotation autour de forward
		changed |= camera_handle_orbit(data);//right en mode roll : autour du centre de la scene
	}
	changed |= camera_handle_dolly(data);
	changed |= camera_handle_level(data);
	if (!changed)
		return ;
	display->flag_camera_changed = 1;
}
