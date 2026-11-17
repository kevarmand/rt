/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_receive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:13:40 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/21 08:46:43 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sync.h"

static void	receive_render_status(t_display *display,
		t_display_mailbox *mailbox, t_frame *frame, int full)
{
	if (!mailbox->snap_is_render)
		return ;
	if (full && mailbox->req_job_id == mailbox->snap_job_id)
		display->ds_sync.in_flight = 0;
	if (frame->requested_job_id != mailbox->snap_job_id)
		return ;
	frame->tiles_received = mailbox->tiles_done;
	frame->tiles_total = mailbox->tile_count;
	if (full)
		frame->quality = mailbox->snapshot_render_mode;
	if (display->ui.cam_info)
		display->flag_img_window = 1;
}

void	display_engine_receive(t_display *display,
				t_display_mailbox *mailbox)
{
	int		camera_id;
	int		full;
	t_frame	*frame;

	if (atomic_load(&mailbox->snapshot_ready) == 0)
		return ;
	camera_id = mailbox->snap_camera_id;
	frame = &display->frame[camera_id];
	full = (mailbox->tiles_done >= mailbox->tile_count);
	receive_render_status(display, mailbox, frame, full);
	if (mailbox->snap_post_settings == display->post_word
		&& mailbox->snap_job_id >= frame->rgb_job_id
		&& (full || display->current_cam == camera_id))
	{
		ft_memcpy(frame->rgb_pixels, mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		frame->rgb_job_id = mailbox->snap_job_id;
		if (display->current_cam == camera_id)
			display->flag_img_buffer = 1;
	}
	atomic_store(&mailbox->snapshot_ready, 0);
}
