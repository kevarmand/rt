/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:23:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:45:11 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"
#include "mlx.h"
#include "errors.h"
#include "keycode.h"
#include <stdio.h>
#include "libft.h"

int	display_exit(t_data *data)
{
	mlx_loop_end(data->display.mlx);
	return (0);
}

void	key_hook_qual(int keycode, t_data *data)
{
	if (keycode == KEY_F1)
	{
		data->display.user_render_mode = USER_RENDER_LOCK_FAST;
		data->display.flag_camera_changed = 1;
	}
	if (keycode == KEY_F2)
	{
		data->display.user_render_mode = USER_RENDER_LOCK_NORMAL;
		data->display.flag_camera_changed = 1;
	}
	if (keycode == KEY_F3)
	{
		data->display.user_render_mode = USER_RENDER_LOCK_SUPER;
		data->display.flag_camera_changed = 1;
	}
	if (keycode == KEY_F4)
	{
		data->display.user_render_mode = USER_RENDER_AUTO;
		data->display.flag_camera_changed = 1;
	}
}

void	key_hook_2(int keycode, t_data *data)
{
	int	active_cam;

	if (keycode == KEY_I)
	{
		data->display.ui.cam_info = !data->display.ui.cam_info;
		data->display.flag_img_buffer = 1;
	}
	if (keycode == KEY_ENTER)
	{
		active_cam = data->display.current_cam;
		printf("Camera %d/%d\n", active_cam, data->display.total_cams);
		printf("Position : (%.3f, %.3f, %.3f)\n",
			data->scene.cameras[active_cam].origin.x,
			data->scene.cameras[active_cam].origin.y,
			data->scene.cameras[active_cam].origin.z);
		printf("Orientation : (%f, %f, %f)\n",
			data->scene.cameras[active_cam].forward.x,
			data->scene.cameras[active_cam].forward.y,
			data->scene.cameras[active_cam].forward.z);
	}
}

int	key_hook(int keycode, t_data *data)
{
	if (keycode == KEY_ESCAPE)
		return (display_exit(data));
	if (keycode == KEY_TAB)
	{
		data->display.ui.visible = !data->display.ui.visible;
		data->display.flag_img_buffer = 1;
	}
	if (keycode == KEY_PLUS)
		data->display.cam_ctrl.nav_mul *= 1.2f;
	if (keycode == KEY_MOINS)
		data->display.cam_ctrl.nav_mul /= 1.2f;
	if (keycode == KEY_CTRL)
		data->display.cam_ctrl.mode = !(data->display.cam_ctrl.mode);
	if (keycode == KEY_SPACE)
	{
		data->display.current_cam++;
		if (data->display.current_cam >= data->display.total_cams)
			data->display.current_cam = 0;
		data->display.flag_img_buffer = 1;
	}
	key_hook_2(keycode, data);
	key_hook_qual(keycode, data);
	return (SUCCESS);
}
