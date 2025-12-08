/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_sync_display.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:26:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/08 13:32:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "display.h"
#include "new_rt.h"
#include "libft.h"
#include <stdatomic.h>

static void	display_engine_receive(t_display *display,
				t_display_mailbox *mailbox)
{
	int	is_full;
	int	show_partial;

	if (atomic_load(&mailbox->snapshot_ready) == 0)
		return ;
	is_full = (mailbox->tiles_done == mailbox->tile_count);
	show_partial = (display->cam_to_render == display->current_cam);
	if (show_partial || is_full)
	{
		ft_memcpy(display->display_pixels, mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		display->flag_img = 1;
		display->flag_ui = 1;
	}
	if (is_full && display->cam_to_render >= 0)
	{
		ft_memcpy(display->frame[display->cam_to_render].rgb_pixels,
			display->display_pixels,
			(size_t)display->pixel_count * sizeof(int));
		display->frame[display->cam_to_render].is_dirty = 0;
		display->cam_to_render = -1;
	}
	atomic_store(&mailbox->snapshot_ready, 0);
}

static int	find_next_dirty_camera(t_display *display, int start_index)
{
	int	index;

	index = start_index;
	while (1)
	{
		if (display->frame[index].is_dirty != 0)
			return (index);
		index++;
		if (index >= display->total_cams)
			index = 0;
		if (index == start_index)
			break ;
	}
	return (-1);
}

static void	display_engine_send(t_scene *scene,
				t_display_mailbox *mailbox,
				t_display *display)
{
	int	camera_id;
	int	start_index;

	if (atomic_load(&mailbox->request_ready) != 0)
		return ;
	if (display->cam_to_render != -1)
		return ;
	camera_id = -1;
	if (display->frame[display->current_cam].is_dirty != 0)
		camera_id = display->current_cam;
	else
	{
		start_index = display->current_cam + 1;
		if (start_index >= display->total_cams)
			start_index = 0;
		camera_id = find_next_dirty_camera(display, start_index);
	}
	if (camera_id < 0)
		return ;
	mailbox->cam = scene->cameras[camera_id];
	display->cam_to_render = camera_id;
	atomic_store(&mailbox->request_ready, 1);
}

void	engine_sync_display(t_data *data)
{
	t_display			*display;
	t_display_mailbox	*mailbox;

	display = &data->display;
	mailbox = &data->engine.render.mailbox;
	display_engine_receive(display, mailbox);
	display_engine_send(&data->scene, mailbox, display);
}
