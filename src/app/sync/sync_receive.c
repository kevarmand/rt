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
	frame->tiles_received = mailbox->tiles_done;
	frame->tiles_total = mailbox->tile_count;
	if (frame->tiles_received == frame->tiles_total)
	{
		frame->quality = mailbox->snapshot_render_mode;
		frame->auto_ev = mailbox->snap_auto_ev;
		frame->state = FRAME_READY;
		if (display->ds_sync.in_flight == mailbox->snap_job_id)
			display->ds_sync.in_flight = 0;
	}
}

void	display_engine_receive(t_display *display,
				t_display_mailbox *mailbox)
{
	t_frame	*frame;
	int		returned;

	if (!atomic_load(&mailbox->snapshot_ready))
		return ;
	frame = &display->frame[mailbox->snap_camera_id];
	/* MLX calls XSync before frame_tick. Return the displaced image only
	 * after changing the reference used by all future window redraws. */
	returned = frame->image_id;
	frame->image_id = mailbox->image_id;
	mailbox->image_id = returned;
	if (display->current_cam == mailbox->snap_camera_id || display->ui.cam_info)
		display->flag_img_window = 1;
	if (snapshot_matches(display, mailbox, frame)
		&& (frame->state != FRAME_READY
			|| mailbox->tiles_done == mailbox->tile_count))
		accept_snapshot(display, mailbox, frame);
	atomic_store(&mailbox->snapshot_ready, 0);
}
