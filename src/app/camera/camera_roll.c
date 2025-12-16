/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_roll.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 22:30:40 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 22:54:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

static void	camera_apply_roll(t_scene *scene, t_display *display, float angle)
{
	t_camera	*camera;

	if (angle == 0.0f)
		return ;
	camera = &scene->cameras[display->current_cam];
	camera->right = vec3f_rot_axis(camera->right,
			camera->forward, angle);
	camera->right = vec3f_normalize(camera->right);
}

int	camera_handle_roll(t_data *data)
{
	t_display		*display;
	t_cam_ctrl		*ctrl;
	t_mouse_state	*mouse;
	float			angle;
	float			delta;

	display = &data->display;
	ctrl = &display->cam_ctrl;
	mouse = &display->mouse;
	if (mouse->is_down == 0)
		ctrl->roll_prev_angle = 0.0f;
	if (mouse->is_down == 0 || mouse->button != MOUSE_BUTTON_RIGHT)
		return (0);
	angle = camera_roll_angle(data, mouse);
	if (angle == 0.0f)
		return (0);
	if (ctrl->roll_prev_angle == 0.0f)
		return (ctrl->roll_prev_angle = angle, 0);
	delta = angle - ctrl->roll_prev_angle;
	if (delta == 0.0f)
		return (0);
	ctrl->roll_prev_angle = angle;
	camera_apply_roll(&data->scene, display, -delta);
	return (1);
}
