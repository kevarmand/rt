/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_tick.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:14:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 16:05:10 by kearmand         ###   ########.fr       */
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
#include <math.h>

static void	display_refresh_main_image(t_data *data)
{
	t_display	*display;
	t_image		*image;
	int			byte_count;
	int			*src;

	if (data->display.flag_img_buffer == 0)
		return ;
	display = &data->display;
	image = &display->main_img;
	src = display->display_pixels;
	byte_count = display->pixel_count * sizeof(int);
	ft_memcpy(image->data, src, (size_t)byte_count);
	display->flag_img_buffer = 0;
	display->flag_img_window = 1;
}


void	display_draw_base(t_data *data)
{
	if (data->display.flag_img_window == 0)
		return ;
	mlx_put_image_to_window(
		data->display.mlx,
		data->display.win,
		data->display.main_img.img_ptr,
		0, 0);
	data->display.flag_img_window = 0;
}

static void	display_draw_ui(t_data *data)
{
	if (!data->display.flag_ui)
		return ;
	//fait des truc en attendant
	data->display.flag_ui = 0;
	(void)data;
}

static void	display_update_ui(t_data *data)
{
	(void)data;
	//si l ui est recalculer il mofiera les champ display.flag_ui et display.flag_img
}

static void	display_prepare_render(t_data *data)
{
	t_display	*display;

	display = &data->display;
	if (!display->flag_camera_changed)
		return ;
	display->flag_request_render = 1;
	display->flag_camera_changed = 0;
}



int frame_tick(t_data *data)
{
	display_update_camera(data);
	engine_sync_display(data);
	display_refresh_main_image(data);
	display_update_ui(data);
	display_draw_base(data);
	display_draw_ui(data);
	return (0);
}

