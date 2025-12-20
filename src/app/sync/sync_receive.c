/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_receive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:13:40 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/20 12:52:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sync.h"

// static void	print_info(const t_display_mailbox *mailbox)
// {
// 	ft_printf("Receives snapshot: cam %d job_id %d mode %d tiles_done %d/%d\n",
// 		mailbox->snap_camera_id, mailbox->snap_job_id,
// 		mailbox->snapshot_render_mode, mailbox->tiles_done,
// 		mailbox->tile_count);
// }

void	display_engine_receive(t_display *display,
				t_display_mailbox *mailbox)
{
	int	camera_id;
	int	is_full_frame;
	int	snapshot_job_id;
	int	snapshot_mode;

	if (atomic_load(&mailbox->snapshot_ready) == 0)
		return ;
	camera_id = mailbox->snap_camera_id;
	snapshot_job_id = mailbox->snap_job_id;
	snapshot_mode = mailbox->snapshot_render_mode;
	display->frame[camera_id].tiles_received = mailbox->tiles_done;
	display->frame[camera_id].tiles_total = mailbox->tile_count;
	is_full_frame = (mailbox->tiles_done >= mailbox->tile_count);
	if (is_full_frame || display->current_cam == camera_id)
	{
		ft_memcpy(display->frame[camera_id].rgb_pixels, mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		if (is_full_frame)
			display->frame[camera_id].quality = snapshot_mode;
		if (display->current_cam == camera_id)
			display->flag_img_buffer = 1;
	}
	if (is_full_frame && mailbox->req_job_id == snapshot_job_id)
		display->ds_sync.in_flight = 0;
	atomic_store(&mailbox->snapshot_ready, 0);
}
