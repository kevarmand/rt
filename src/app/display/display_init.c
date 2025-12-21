/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 18:42:36 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 07:25:50 by kearmand         ###   ########.fr       */
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
#include "logs.h"
#include "cam_ctrl.h"

int		display_init_frames(t_display *display, const t_scene *scene);
int		init_cam_ctrl(t_data *data);
void	init_sky_texture(t_scene *scene);
int		xpm_load(t_scene *scene, t_display *display);

static int	init_core(t_display *display, int width, int height)
{
	display->mlx = mlx_init();
	if (!display->mlx)
		return (perr(ERR_MLX, PERR_MLX_INIT));
	display->win = mlx_new_window(display->mlx, width, height, RT_NAME);
	if (!display->win)
	{
		mlx_destroy_display(display->mlx);
		free(display->mlx);
		return (perr(ERR_MLX, PERR_MLX_WIN));
	}
	display->main_img.img_ptr = mlx_new_image(display->mlx, width, height);
	if (!display->main_img.img_ptr)
	{
		mlx_destroy_window(display->mlx, display->win);
		mlx_destroy_display(display->mlx);
		free(display->mlx);
		return (perr(ERR_MLX, PERR_MLX_IMG));
	}
	display->main_img.data = mlx_get_data_addr(display->main_img.img_ptr,
			&display->main_img.bpp,
			&display->main_img.size_l,
			&display->main_img.endian);
	display->main_img.width = width;
	display->main_img.height = height;
	return (SUCCESS);
}

int	ft_exit(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	mlx_loop_end(data->display.mlx);
	return (0);
}

static int	init_loop(t_display *display, t_data *data)
{
	mlx_hook(display->win, DestroyNotify, 0, ft_exit, data);
	mlx_key_hook(display->win, &key_hook, data);
	mlx_loop_hook(display->mlx, frame_tick, data);
	mlx_hook(display->win, 4, 1L << 2, mouse_press, data);
	mlx_hook(display->win, 5, 1L << 3, mouse_release, data);
	mlx_hook(display->win, 6, 1L << 6, mouse_move, data);
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
	display->user_render_mode = USER_RENDER_AUTO;
	display->render_ssaa = 1;
	display->render_tonemap = 1;
	display->cam_ctrl.mode = CAM_MODE_LOCK;
}

int	display_init(t_display *display, t_data *data)
{
	int	status;
	int	pixel_count;

	pixel_count = data->engine.width * data->engine.height;
	display_reset_struct(display, pixel_count);
	status = init_core(display, data->engine.width, data->engine.height);
	if (status != SUCCESS)
		display->mlx = NULL;
	log_step(LOGSTEP_DISPLAY_INIT_CORE, status);
	if (status == SUCCESS)
		status = display_init_frames(display, &data->scene);
	log_step(LOGSTEP_DISPLAY_INIT_FRAMES, status);
	if (status == SUCCESS)
		status = init_loop(display, data);
	log_step(LOGSTEP_DISPLAY_INIT_LOOP, status);
	if (status == SUCCESS)
		status = xpm_load(&data->scene, display);
	log_step(LOGSTEP_DISPLAY_LOAD_TEXTURES, status);
	if (status == SUCCESS)
		status = init_cam_ctrl(data);
	log_step(LOGSTEP_DISPLAY_INIT_CAM_CTRL, status);
	if (status == SUCCESS)
		init_sky_texture(&data->scene);
	return (status);
}
