/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 18:42:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/23 20:40:56 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"
#include "errors.h"
#include "mlx.h"
#include "rt_config.h"
#include "new_rt.h"
#include "engine.h"

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

static int	init_loop(t_display *display, t_data *data)
{
	mlx_hook(data->win_ptr, 33, StructureNotifyMask, &ft_exit, data);
	mlx_key_hook(data->win_ptr, &key_hook, data);
	mlx_loop_hook(data->mlx_ptr, &frame_tick, data);
	mlx_hook(data->win_ptr, 4, 1L << 2, mouse_press, data);      // ButtonPress
	mlx_hook(data->win_ptr, 5, 1L << 3, mouse_release, data);    // ButtonRelease
	mlx_hook(data->win_ptr, 6, 1L << 6, mouse_move, data);       // MotionNotify
	
	return (SUCCESS);
}

int	display_init(t_display *display, t_data *data))
{
	int	status;
	memset(display, 0, sizeof(t_display));
	status  = init_core(display, data->engine.width, data->engine.height);
	if (status != SUCCESS)
		return (status);
	status = init_loop(display, data);
	if (status != SUCCESS)
		return (status);
	status = ui_init(&display->ui, display->mlx, display->win, data->engine.width, data->engine.height);
	return (status);
}