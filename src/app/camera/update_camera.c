/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 23:13:26 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 21:00:33 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

void	display_update_camera(t_data *data)
{
	t_display	*display;
	int			changed;

	display = &data->display;
	changed = 0;
	if (display->cam_ctrl.mode == CAM_MODE_STANDARD)
	{
		changed |= camera_handle_orbit(data);
		changed |= camera_handle_pan(data);
	}
	else if (display->cam_ctrl.mode == CAM_MODE_ROLL)
	{
		changed |= camera_handle_look(data);
		changed |= camera_handle_roll(data);
	}
	changed |= camera_handle_dolly(data);
	changed |= camera_handle_level(data);
	if (!changed)
		return ;
	display->flag_camera_changed = 1;
}
