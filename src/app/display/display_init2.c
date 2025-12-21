/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_init2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 21:50:45 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:29:24 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"
#include "errors.h"
#include "new_rt.h"
#include "libft.h"
#include "vector.h"
#include <math.h>

int	display_init_frames(t_display *display, const t_scene *scene)
{
	int		index;
	int		camera_count;
	size_t	bytes;

	camera_count = scene->camera_count;
	display->total_cams = camera_count;
	display->frame = malloc(sizeof(t_frame) * (size_t)camera_count);
	if (!display->frame)
		return (perr(ERR_MALLOC, PERR_M_DISPLAY_FRAME));
	display->pixel_count = scene->resolution_width * scene->resolution_height;
	bytes = sizeof(int) * (size_t)display->pixel_count;
	ft_memset(display->frame, 0, sizeof(t_frame) * (size_t)camera_count);
	index = 0;
	while (index < camera_count)
	{
		display->frame[index].rgb_pixels = malloc(bytes);
		if (!display->frame[index].rgb_pixels)
			return (perr(ERR_MALLOC, PERR_M_DISPLAY_FRAME_PIX));
		ft_memset(display->frame[index].rgb_pixels, 0, bytes);
		display->frame[index].quality = QUALITY_NONE;
		index++;
	}
	display->current_cam = 0;
	display->flag_camera_changed = 1;
	return (SUCCESS);
}

static float	max3f(float value_a, float value_b, float value_c)
{
	float	max_value;

	max_value = value_a;
	if (value_b > max_value)
		max_value = value_b;
	if (value_c > max_value)
		max_value = value_c;
	return (max_value);
}

int	init_cam_ctrl(t_data *data)
{
	t_display	*display;
	t_aabb		bounds;
	t_vec3f		extent;

	display = &data->display;
	display->cam_ctrl.mode = CAM_MODE_STANDARD;
	display->cam_ctrl.nav_mul = 1.0f;
	if (data->scene.bvh_node_count == 0)
	{
		display->cam_ctrl.nav_scale = 1.0f;
		display->cam_ctrl.center = (t_vec3f){0.0f, 0.0f, 0.0f};
		return (SUCCESS);
	}
	bounds = data->scene.bvh_nodes[0].bounds;
	extent = vec3f_sub(bounds.b[1], bounds.b[0]);
	extent.x = fabsf(extent.x);
	extent.y = fabsf(extent.y);
	extent.z = fabsf(extent.z);
	display->cam_ctrl.nav_scale = max3f(extent.x, extent.y, extent.z);
	if (display->cam_ctrl.nav_scale < 0.001f)
		display->cam_ctrl.nav_scale = 0.001f;
	display->cam_ctrl.center = vec3f_scale(vec3f_add(bounds.b[0],
				bounds.b[1]), 0.5f);
	return (SUCCESS);
}

void	init_sky_texture(t_scene *scene)
{
	t_texture	*tex;
	t_vec3f		intensity;
	int			index;

	if (scene->skybox.texture_id != SCENE_ID_NONE)
	{
		index = 0;
		intensity = scene->skybox.intensity;
		tex = &scene->textures[scene->skybox.texture_id];
		while (index < tex->width * tex->height)
		{
			((t_vec3f *)tex->pixels)[index] = vec3f_mul(((
							t_vec3f *)tex->pixels)[index], intensity);
			index++;
		}
	}
}
