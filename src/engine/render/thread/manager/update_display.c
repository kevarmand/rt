/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_display.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 18:39:42 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:06:47 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include "libft.h"
#include <stdio.h>

int	manager_update_display(t_render *render)
{
	t_mgr				*manager;
	t_display_mailbox	*mailbox;
	int					pixel_count;

	manager = &render->manager;
	mailbox = &render->mailbox;
	if (atomic_load(&mailbox->snapshot_ready) != 0)
		return (0);
	pixel_count = render->width * render->height;
	ft_memcpy(mailbox->rgb_pixels, manager->rgb_buffer,
		(size_t)pixel_count * sizeof(int));
	mailbox->tile_count = manager->tileset.tiles_total;
	mailbox->tiles_done = manager->tileset.tiles_done;
	mailbox->snap_job_id = manager->render_view.frame_seq;
	mailbox->snap_camera_id = manager->current_cam_id;
	mailbox->snapshot_render_mode = manager->render_view.mode;
	atomic_store(&mailbox->snapshot_ready, 1);
	if (manager->tileset.tiles_done == manager->tileset.tiles_total)
	{
		manager->render_in_progress = 0;
	}
	return (1);
}
