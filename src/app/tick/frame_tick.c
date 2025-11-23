/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_tick.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:14:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 20:24:49 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "new_rt.h"
#include "display.h"
#include "render.h"
#include "ui.h"
#include "engine.h"

static void	display_update_main_image(t_data *data, int image_modified)
{
	if (!image_modified)
		return ;
	buffer_to_mlx_image(&data->display.main_img,
		data->engine.render.rgb_front,
		data->engine.render.width);
	display_flag_set(&data->display.flag, DF_UI);
}

static void	display_update_ui(t_data *data)
{
	if (!display_flag_has(data->display.flag, DF_UI))
		return ;
	reset_ui(data);
}

static void	display_draw_base(t_data *data)
{
	if (!display_flag_has(data->display.flag, DF_UI))
		return ;
	mlx_put_image_to_window(data->display.mlx, data->display.win,
		data->display.main_img.img_ptr, 0, 0);
}

static void	display_draw_ui(t_data *data)
{
	if (!data->display.ui.visible)
		return ;
	if (display_flag_has(data->display.flag, DF_UI))
		mlx_put_image_to_window(data->display.mlx, data->display.win,
			data->display.ui.img.img_ptr,
			data->display.ui.x, data->display.ui.y);
	if (display_flag_has(data->display.flag, DF_TEXT))
		print_txt_ui(data);
}

int	frame_tick(t_data *data)
{
	int	image_modified;

	image_modified = engine_tick(&data->engine);
	display_update_main_image(data, image_modified);
	display_update_ui(data);
	display_draw_base(data);
	display_draw_ui(data);
	display_flag_clear(&data->display.flag, DF_ALL);
	return (0);
}
