/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_send.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:29:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 16:35:12 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sync.h"

static void	prepare_request(t_display *display, t_display_mailbox *mailbox)
{
	t_ds_sync	*sync;
	t_frame		*frame;

	sync = &display->ds_sync;
	if (sync->in_flight)
		display_invalidate_frame(display, mailbox, sync->running_cam);
	frame = &display->frame[sync->req_cam];
	mailbox->req_job_id++;
	frame->state = FRAME_RENDERING;
	frame->tiles_received = 0;
	atomic_store(&mailbox->expected_seq[sync->req_cam], mailbox->req_job_id);
	sync->in_flight = mailbox->req_job_id;
	sync->running_cam = sync->req_cam;
	display->flag_img_window = 1;
}

void	display_engine_send(t_scene *scene, t_display *display,
				t_display_mailbox *mailbox)
{
	t_ds_sync	*sync;
	int			camera_index;

	sync = &display->ds_sync;
	if (!sync->pending || atomic_load(&mailbox->request_ready))
		return ;
	camera_index = sync->req_cam;
	prepare_request(display, mailbox);
	mailbox->cam = scene->cameras[camera_index];
	mailbox->request_camera_id = camera_index;
	mailbox->request_render_mode = sync->req_mode;
	mailbox->request_ssaa = display->render_ssaa;
	atomic_store(&mailbox->request_ready, 1);
	sync->pending = 0;
}
