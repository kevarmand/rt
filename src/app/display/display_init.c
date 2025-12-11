/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 18:42:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 21:56:55 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"
#include "errors.h"
#include "mlx.h"
#include "rt_config.h"
#include "new_rt.h"
#include "engine.h"
#include "app.h"
#include <X11/X.h>
#include <stdlib.h>
#include "libft.h"


static int	init_core(t_display *display, int width, int height)
{
	display->mlx = mlx_init();
	if (!display->mlx)
		return (ERR_INTERNAL);
	display->win = mlx_new_window(display->mlx, width, height, RT_NAME);
	if (!display->win)
	{
		mlx_destroy_display(display->mlx);
		free(display->mlx);
		return (ERR_INTERNAL);
	}
	display->main_img.img_ptr = mlx_new_image(display->mlx, width, height);
	if (!display->main_img.img_ptr)
	{
		mlx_destroy_window(display->mlx, display->win);
		mlx_destroy_display(display->mlx);
		free(display->mlx);
		return (ERR_INTERNAL);
	}
	display->main_img.data = mlx_get_data_addr(display->main_img.img_ptr,
			&display->main_img.bpp,
			&display->main_img.size_l,
			&display->main_img.endian);
	display->main_img.width = width;
	display->main_img.height = height;
	return (SUCCESS);
}
int ft_exit(void *param)
{
	t_data *data;
	
	data = (t_data *)param;
	mlx_loop_end(data->display.mlx);
	return (0);
}

static int	init_loop(t_display *display, t_data *data)
{
	//mlx_hook(display->win, 33, StructureNotifyMask, &ft_exit, data);
	mlx_hook(display->win, DestroyNotify, 0, ft_exit, data);
	mlx_key_hook(display->win, &key_hook, data);
	mlx_loop_hook(display->mlx, &frame_tick, data);
	mlx_hook(display->win, 4, 1L << 2, mouse_press, data);
	mlx_hook(display->win, 5, 1L << 3, mouse_release, data);
	mlx_hook(display->win, 6, 1L << 6, mouse_move, data);
	return (SUCCESS);
}

static int	display_init_frames(t_display *display, const t_scene *scene)
{
	int		index;
	int		camera_count;
	size_t	bytes;

	camera_count = scene->camera_count;
	display->total_cams = camera_count;
	display->frame = malloc(sizeof(t_frame) * (size_t)camera_count);
	if (!display->frame)
		return (ERR_MALLOC);
	bytes = sizeof(int) * (size_t)display->pixel_count;
	index = 0;
	while (index < camera_count)
	{
		display->frame[index].rgb_pixels = malloc(bytes);
		if (!display->frame[index].rgb_pixels)
			return (ERR_MALLOC);
		ft_memset(display->frame[index].rgb_pixels, 0, bytes);
		display->frame[index].quality = QUALITY_NONE;
		index++;
	}
	display->current_cam = 0;
	display->cam_to_render = -1;
	display->flag_camera_changed = 1;
	return (SUCCESS);
}

static void	display_reset_struct(t_display *display, int pixel_count)
{
	ft_memset(display, 0, sizeof(t_display));
	display->pixel_count = pixel_count;
	display->flag_img_buffer = 1;
	display->flag_img_window = 1;
	display->flag_camera_changed = 1;
	display->flag_ui = 1;
	display->user_render_mode = USER_RENDER_LOCK_SUPER;
}

int	load_scene_textures(t_scene *scene,t_display *display);

int	display_init(t_display *display, t_data *data)
{
	int	status;
	int	pixel_count;

	pixel_count = data->engine.width * data->engine.height;
	display_reset_struct(display, pixel_count);
	status = init_core(display, data->engine.width, data->engine.height);
	// if (status == SUCCESS)
	// 	status = ui_init(display);
	if (status == SUCCESS)
		status = display_init_frames(display, &data->scene);
	if (status == SUCCESS)
		status = init_loop(display, data);
	if (status == SUCCESS)
		status = load_scene_textures(&data->scene, display);
	return (status);
}
