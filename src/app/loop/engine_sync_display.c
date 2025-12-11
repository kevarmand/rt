/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_sync_display.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 20:26:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 17:06:07 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "display.h"
#include "new_rt.h"
#include "libft.h"
#include <stdatomic.h>

static int	helper_mode(int is_full, int render_mode)
{
	if (is_full)
	{
		if (render_mode == FAST_MODE)
			return (FRAME_STATE_FAST_FULL);
		else if (render_mode == NORMAL_MODE)
			return (FRAME_STATE_NORMAL_FULL);
		else if (render_mode == SUPER_MODE)
			return (FRAME_STATE_SUPER_FULL);
	}
	return (FRAME_STATE_DIRTY);
}

static void	display_engine_receive(t_display *display,
				t_display_mailbox *mailbox)
{
	int	camera_id;
	int	is_full;
	int	job_id;
	int	mode;

	if (atomic_load(&mailbox->snapshot_ready) == 0)
		return ;
	camera_id = mailbox->snap_camera_id;
	job_id = mailbox->snap_job_id;
	mode = mailbox->snapshot_render_mode;
	is_full = (mailbox->tiles_done >= mailbox->tile_count);
	if (is_full || display->current_cam == camera_id)
	{
		ft_memcpy(display->frame[camera_id].rgb_pixels,
			mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		display->frame[camera_id].state = helper_mode(is_full, mode);
		if (display->current_cam == camera_id)
			display->flag_img_buffer = 1;
	}
	if (is_full && mailbox->req_job_id == job_id)
		display->ds_sync.in_flight = 0;
	atomic_store(&mailbox->snapshot_ready, 0);
}

static int	find_next_dirty_camera(t_display *display)
{
	int	index;

	index = 0;
	while (index < display->total_cams)
	{
		if (display->frame[index] != 0)
			return (index);
		index++;
	}
	return (-1);
}

typedef struct s_local_request
{
	int	camera_id;
	int	render_mode;     // FAST_MODE / NORMAL_MODE / SUPER_MODE
	int	is_foreground;   // 1 = current_cam, 0 = background
	int	request_render;  // 1 = il y a un job à envoyer, 0 = rien
}	t_local_request;

static void	display_engine_send(t_scene *scene, t_display *display,
				t_display_mailbox *mailbox)
{
	int	ready;
	int	camera_id;

	if (req->request_render == 0)
		return ;
	ready = atomic_load(&mailbox->request_ready);
	if (ready != 0)
		return ;
	camera_id = req->camera_id;
	mailbox->cam = scene->cameras[camera_id];
	mailbox->request_camera_id = camera_id;
	mailbox->request_render_mode = req->render_mode;
	mailbox->req_job_id++;
	atomic_store(&mailbox->request_ready, 1);
	display->ds_sync.in_flight = 1;
	display->ds_sync.pending = 0;
}


void	engine_sync_display(t_data *data)
{
	t_display			*display;
	t_display_mailbox	*mailbox;
	t_local_request		req;

	display = &data->display;
	mailbox = &data->engine.render.mailbox;
	req.request_render = 0;
	display_engine_receive(display, mailbox);
	display_request_policy(&data->scene, display, &req);
	if (req.request_render != 0)
		display_engine_send(&data->scene, display, mailbox, &req);
}

