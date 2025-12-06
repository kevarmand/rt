/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 19:19:23 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/06 19:19:26 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "display.h"

void	display_mark_current_cam_dirty(t_display *display)
{
	int	index;

	index = display->current_cam;
	display->frame[index].is_dirty = 1;
	display->flag_camera_changed = 1;
	display->flag_img = 1;
}

void	display_set_current_cam(t_display *display, int new_index)
{
	if (new_index == display->current_cam)
		return ;
	display->current_cam = new_index;
	display->flag_camera_changed = 1;
	display->flag_img = 1;
}

void	display_mark_cam_clean(t_display *display, int cam_index)
{
	display->frame[cam_index].is_dirty = 0;
	if (display->cam_to_render == cam_index)
		display->cam_to_render = -1;
}
