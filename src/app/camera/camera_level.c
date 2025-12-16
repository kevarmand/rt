/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_level.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 22:33:14 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 22:35:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

static t_vec3f	camera_lvl_right(t_vec3f world_up, t_vec3f forward,
					t_vec3f fallback)
{
	t_vec3f	right;
	float	len2;

	right = vec3f_cross(world_up, forward);
	len2 = vec3f_dot(right, right);
	if (len2 < EPSILON)
		right = fallback;
	return (vec3f_normalize(right));
}

int	camera_handle_level(t_data *data)
{
	t_display	*display;
	t_camera	*camera;
	t_cam_ctrl	*ctrl;
	t_vec3f		world_up;
	t_vec3f		forward;

	display = &data->display;
	ctrl = &display->cam_ctrl;
	if (!ctrl->flag_level)
		return (0);
	camera = &data->scene.cameras[display->current_cam];
	world_up = (t_vec3f){0.0f, -1.0f, 0.0f};
	forward = vec3f_sub(ctrl->center, camera->origin);
	camera->forward = vec3f_normalize(forward);
	camera->right = camera_lvl_right(world_up, camera->forward, camera->right);
	ctrl->flag_level = 0;
	return (1);
}
