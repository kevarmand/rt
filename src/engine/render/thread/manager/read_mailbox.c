/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_mailbox.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 18:47:20 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/26 19:43:16 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include "render.h"
#include "bitmap.h"

static void	manager_apply_cam_request(t_render *render, const t_camera *cam)
{
	t_mgr			*manager;
	t_render_view	*view;

	manager = &render->manager;
	view = &manager->render_view;
	copy_request(view, cam);
	init_cam_vectors(view);
	manager->render_in_progress = 1;
	manager->tileset.tiles_done = 0;
	bitmap_zero(&manager->tileset.tile_state);
	manager->render_view.frame_seq++;
	manager->tileset.tiles_active = 0;
}

int	manager_read_mailbox(t_render *render)
{
	t_mgr				*manager;
	t_display_mailbox	*mailbox;

	manager = &render->manager;
	mailbox = &render->mailbox;
	if (atomic_load(&mailbox->request_ready) == 0)
		return (0);
	manager_apply_cam_request(render, &mailbox->cam);
	atomic_store(&mailbox->request_ready, 0);
	return (1);
}
