/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_sync_display.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:26:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/09 19:54:17 by kearmand         ###   ########.fr       */
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
	int	camera_id;
	int	is_full;
	int	job_id;

	if (atomic_load(&mailbox->snapshot_ready) == 0)
		return ;
	camera_id = display->cam_to_render;
	if (camera_id == -1)
	{
		atomic_store(&mailbox->snapshot_ready, 0);
		return ;
	}
	job_id = mailbox->snap_job_id;
	is_full = (mailbox->tiles_done >= mailbox->tile_count);
	if (display->background_job == 0
		&& camera_id == display->current_cam)
	{
		ft_memcpy(display->display_pixels, mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		display->flag_img_buffer = 1;
	}
	if (is_full && job_id == mailbox->req_job_id)
	{
		ft_memcpy(display->frame[camera_id].rgb_pixels,
			mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		display->frame[camera_id].is_dirty = 0;
		display->cam_to_render = -1;
		display->background_job = 0;
	}
	printf("Received snapshot for camera %d (job %d), full=%d\n",
		camera_id, job_id, is_full);
	atomic_store(&mailbox->snapshot_ready, 0);
}


static int	find_next_dirty_camera(t_display *display)
{
	int	index;

	index = 0;
	while (index < display->total_cams)
	{
		if (display->frame[index].is_dirty != 0)
			return (index);
		index++;
	}
	return (-1);
}

static void	display_engine_send_foreground(t_scene *scene,
				t_display_mailbox *mailbox,
				t_display *display)
{
	int	ready;
	int	camera_id;

	if (display->flag_request_render == 0)
		return ;
	ready = atomic_load(&mailbox->request_ready);
	if (ready != 0)
		return ;
	camera_id = display->current_cam;
	display->cam_to_render = camera_id;
	display->frame[camera_id].is_dirty = 1;
	display->background_job = 0;
	mailbox->cam = scene->cameras[camera_id];
	mailbox->req_job_id++;
	atomic_store(&mailbox->request_ready, 1);
	display->flag_request_render = 0;
}

static void	display_engine_send_background(t_scene *scene,
				t_display_mailbox *mailbox,
				t_display *display)
{
	int	ready;
	int	camera_id;

	ready = atomic_load(&mailbox->request_ready);
	if (ready != 0 || display->cam_to_render != -1)
		return ;
	camera_id = find_next_dirty_camera(display);
	if (camera_id < 0)
		return ;
	display->cam_to_render = camera_id;
	display->background_job = 1;
	mailbox->cam = scene->cameras[camera_id];
	mailbox->req_job_id++;
	atomic_store(&mailbox->request_ready, 1);
}

static void	display_engine_send(t_scene *scene,
				t_display_mailbox *mailbox,
				t_display *display)
{
	display_engine_send_foreground(scene, mailbox, display);
	display_engine_send_background(scene, mailbox, display);
}


void	engine_sync_display(t_data *data)
{
	t_display			*display;
	t_display_mailbox	*mailbox;

	display = &data->display;
	mailbox = &data->engine.render.mailbox;
	display_engine_send(&data->scene, mailbox, display);
	display_engine_receive(display, mailbox);

}
