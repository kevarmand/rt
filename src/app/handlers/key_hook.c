/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 19:23:24 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 22:56:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"
#include "mlx.h"
#include "errors.h"
#include "keycode.h"

int	display_exit(t_data *data)
{
	mlx_loop_end(data->display.mlx);  // sort proprement de mlx_loop
	return (0);
}
int	key_hook(int keycode, t_data *data)
{
	if (keycode == KEY_ESCAPE)
		return (display_exit(data));
	if (keycode == KEY_TAB)
		data->display.ui.visible = !data->display.ui.visible;
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
	return (SUCCESS);
}
