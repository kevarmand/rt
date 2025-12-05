/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_tick.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:14:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 23:13:07 by kearmand         ###   ########.fr       */
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



int frame_tick(t_data *data)
{
	int image_changed;

	display_update_camera(data);
	image_changed = engine_sync_display(data);
	if (image_changed)
		display_update_main_image(data);
	display_update_ui(data);
	display_draw_base(data);
	display_draw_ui(data);

	clear_flags(data);
	return (0);
}

