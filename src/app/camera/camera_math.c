/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_math.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 20:41:39 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 22:46:50 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

t_vec3f	vec3f_rot_axis(t_vec3f vector,
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

float	camera_mouse_yaw(const t_scene *scene,
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

float	camera_mouse_pitch(const t_scene *scene,
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

float	camera_roll_angle(const t_data *data, const t_mouse_state *mouse)
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
