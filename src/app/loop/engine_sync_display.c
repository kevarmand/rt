/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_sync_display.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:26:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 13:45:01 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "display.h"
#include "new_rt.h"
#include "libft.h"
#include <stdatomic.h>

typedef enum e_mode
{
	FOREGROUND,
	BACKGROUND
}	t_mode;

static int	sync_snapshot(t_display *display,
				t_display_mailbox *mailbox,
				int mode,
				int *is_full)
{
	int	status;

	*is_full = 0;
	status = 0;
	if (atomic_load(&mailbox->snapshot_ready) == 0)
		return (0);
	if (mode == FOREGROUND
		|| mailbox->tiles_done == mailbox->tile_count)
	{
		ft_memcpy(display->display_pixels, mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		status = 1;
	}
	if (mailbox->tiles_done == mailbox->tile_count)
		*is_full = 1;
	atomic_store(&mailbox->snapshot_ready, 0);
	return (status);
}

static int find_next_background_camera(t_display *display)
{
	int i;
	int start_cam;

	start_cam = (display->cam_to_render + 1) % display->total_cams;
	i = start_cam;
	while (1)
	{
		if (display->frame[i].is_dirty != 0)
			return (i);
		i = (i + 1) % display->total_cams;
		if (i == start_cam)
			break ;
	}
	return (-1);
}

static void	sync_requests(t_scene *scene,
				t_display_mailbox *mailbox,
				t_display *display,
				int mode)
{
	int	camera_id;

	if (atomic_load(&mailbox->request_ready) != 0)
		return ;
	if (mode == FOREGROUND && display->flag_camera_changed != 0)
	{
		camera_id = display->current_cam;
		mailbox->cam = scene->cameras[camera_id];
		display->flag_camera_changed = 0;
		atomic_store(&mailbox->request_ready, 1);
		return ;
	}
	if (mode == BACKGROUND && display->cam_to_render == -1)
	{
		camera_id = find_next_background_camera(display);
		if (camera_id >= 0)
		{
			mailbox->cam = scene->cameras[camera_id];
			display->cam_to_render = camera_id;
			atomic_store(&mailbox->request_ready, 1);
		}
	}
}
static int	apply_snapshot(t_display *display, int mode, int is_full)
{
	int	image_changed;

	image_changed = 0;
	if (mode == FOREGROUND)
	{
		image_changed = 1;
		display->flag_img = 1;
		display->flag_ui = 1;
		if (is_full != 0)
		{
			ft_memcpy(display->frame[display->current_cam].rgb_pixels,
				display->display_pixels,
				(size_t)display->pixel_count * sizeof(int));
			display->frame[display->current_cam].is_dirty = 0;
		}
	}
	else if (is_full != 0 && display->cam_to_render >= 0)
	{
		ft_memcpy(display->frame[display->cam_to_render].rgb_pixels,
			display->display_pixels,
			(size_t)display->pixel_count * sizeof(int));
		display->frame[display->cam_to_render].is_dirty = 0;
		display->cam_to_render = -1;
	}
	return (image_changed);
}

int	engine_sync_display(t_data *data)
{
	t_display			*display;
	t_display_mailbox	*mailbox;
	int					mode;
	int					is_full;
	int					image_changed;

	display = &data->display;
	mailbox = &data->engine.render.mailbox;
	if (display->frame[display->current_cam].is_dirty != 0)
		mode = FOREGROUND;
	else
		mode = BACKGROUND;
	image_changed = 0;
	if (sync_snapshot(display, mailbox, mode, &is_full) != 0)
		image_changed = apply_snapshot(display, mode, is_full);
	sync_requests(&data->scene, mailbox, display, mode);
	return (image_changed);
}
