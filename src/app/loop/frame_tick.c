/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_tick.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:14:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 16:26:49 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "new_rt.h"
#include "display.h"
#include "render.h"
#include "engine.h"
#include "app.h"
#include "mlx.h"
#include "libft.h"
#include <stdio.h>

static void	display_update_main_image(t_data *data)
{
	t_display	*display;
	t_image		*image;
	int			byte_count;

	display = &data->display;
	image = &display->main_img;
	byte_count = display->pixel_count * (int)sizeof(int);
	ft_memcpy(image->data, display->display_pixels,
		(size_t)byte_count);
	display->flag_img = 1;
}


void	display_draw_base(t_data *data)
{
	if (data->display.flag_img == 0)
		return ;
	mlx_put_image_to_window(
		data->display.mlx,
		data->display.win,
		data->display.main_img.img_ptr,
		0, 0);
}

static void	display_draw_ui(t_data *data)
{
	if (!data->display.flag_ui)
		return ;
	//fait des truc en attendant
	(void)data;
}

static void	display_update_ui(t_data *data)
{
	(void)data;
	//si l ui est recalculer il mofiera les champ display.flag_ui et display.flag_img
}

static void	clear_flags(t_data *data)
{
	data->display.flag_img = 0;
	data->display.flag_ui = 0;
	//reset_flag_sepcifique UI
}

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

	camera = &scene->cameras[display->current_cam];
	if (delta_x == 0 && delta_y == 0)
		return ;
	yaw = (float)delta_x * 0.00025f;
	pitch = (float)delta_y * 0.00025f;
	up = vec3f_cross(camera->right, camera->forward);
	up = vec3f_normalize(up);
	if (yaw != 0.0f)
	{
		camera->forward = vec3f_rotate_axis(camera->forward, up, yaw);
		camera->forward = vec3f_normalize(camera->forward);
		camera->right = vec3f_cross(up, camera->forward);
		camera->right = vec3f_normalize(camera->right);
	}
	if (pitch != 0.0f)
	{
		camera->forward = vec3f_rotate_axis(camera->forward,
				camera->right, -pitch);
		camera->forward = vec3f_normalize(camera->forward);
	}
}


static void	display_update_camera(t_data *data)
{
	t_display		*display;
	t_mouse_state	*mouse;
	int				delta_x;
	int				delta_y;

	display = &data->display;
	mouse = &display->mouse;
	if (mouse->mode != 2)
		return ;
	delta_x = mouse->accum_dx;
	delta_y = mouse->accum_dy;
	if (delta_x == 0 && delta_y == 0)
		return ;
	mouse->accum_dx = 0;
	mouse->accum_dy = 0;
	camera_apply_mouse_delta(&data->scene, display, delta_x, delta_y);
	display->frame[display->current_cam].is_dirty = 1;
	display->flag_camera_changed = 1;
}

int frame_tick(t_data *data)
{
	int image_changed;


	image_changed = engine_sync_display(data);
	// if (image_changed)
		display_update_main_image(data);
	display_update_camera(data);
	display_update_ui(data);
	display_draw_base(data);
	display_draw_ui(data);

	clear_flags(data);
	return (0);
}

