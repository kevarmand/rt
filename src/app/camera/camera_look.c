/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_look.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:47:17 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 22:55:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

static void	camera_apply_mouse_delta(t_scene *scene,
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
		camera->forward = vec3f_rot_axis(camera->forward, up, yaw);
		camera->forward = vec3f_normalize(camera->forward);
		camera->right = vec3f_cross(camera->forward, up);
		camera->right = vec3f_normalize(camera->right);
	}
	if (pitch != 0.0f)
	{
		camera->forward = vec3f_rot_axis(camera->forward,
				camera->right, pitch);
		camera->forward = vec3f_normalize(camera->forward);
	}
}

int	camera_handle_look(t_data *data)
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
