/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_tick.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:14:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 09:33:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"
#include "render.h"
#include "app.h"
#include "mlx.h"
#include "libft.h"
#include <stdio.h>
#include <math.h>

void	display_update_ui(t_display *display);
void	print_txt_ui(t_data *data);

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
	src = display->frame[display->current_cam].rgb_pixels;
	byte_count = display->pixel_count * sizeof(int);
	ft_memcpy(image->data, src, byte_count);
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
	t_display	*display;

	display = &data->display;
	if (!display->ui.visible)
		return ;
	mlx_put_image_to_window(display->mlx, display->win,
		display->ui.img.img_ptr, display->ui.x, display->ui.y);
	print_txt_ui(data);
}

int	frame_tick(t_data *data)
{
	display_update_ui(&data->display);
	display_update_camera(data);
	sync_engine_tick(data);
	display_refresh_main_image(data);
	display_draw_base(data);
	display_draw_ui(data);
	if (data->display.ui.cam_info)
		ui_print_cam_status(data);
	return (0);
}
