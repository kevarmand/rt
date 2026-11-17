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

static int	snapshot_matches(const t_display *display,
		const t_display_mailbox *mailbox, const t_frame *frame)
{
	return (mailbox->snap_job_id != 0
		&& mailbox->snap_job_id == atomic_load(
			&mailbox->expected_seq[mailbox->snap_camera_id])
		&& (frame->state == FRAME_RENDERING || frame->state == FRAME_READY)
		&& mailbox->snap_post_settings == display->post_word);
}

static void	accept_snapshot(t_display *display,
		t_display_mailbox *mailbox, t_frame *frame)
{
	ft_memcpy(frame->rgb_pixels, mailbox->rgb_pixels,
		(size_t)display->pixel_count * sizeof(int));
	frame->tiles_received = mailbox->tiles_done;
	frame->tiles_total = mailbox->tile_count;
	if (frame->tiles_received == frame->tiles_total)
	{
		frame->quality = mailbox->snapshot_render_mode;
		frame->state = FRAME_READY;
		if (display->ds_sync.in_flight == mailbox->snap_job_id)
			display->ds_sync.in_flight = 0;
	}
	if (display->current_cam == mailbox->snap_camera_id)
		display->flag_img_buffer = 1;
	if (display->ui.cam_info)
		display->flag_img_window = 1;
}

void	display_engine_receive(t_display *display,
				t_display_mailbox *mailbox)
{
	t_frame	*frame;

	if (!atomic_load(&mailbox->snapshot_ready))
		return ;
	frame = &display->frame[mailbox->snap_camera_id];
	if (snapshot_matches(display, mailbox, frame)
		&& (frame->state != FRAME_READY
			|| mailbox->tiles_done == mailbox->tile_count))
		accept_snapshot(display, mailbox, frame);
	atomic_store(&mailbox->snapshot_ready, 0);
}
