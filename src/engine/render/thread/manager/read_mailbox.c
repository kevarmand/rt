/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_mailbox.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 18:47:20 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/09 19:59:45 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include "render.h"
#include "bitmap.h"
#include <stdio.h>

static void	copy_request(t_render_view *view, const t_camera *src)
{
	view->origin = src->origin;
	view->forward = src->forward;
	view->right = src->right;
	view->fov_deg = src->fov_deg;
	
}
static void	apply_cam_request(t_render *render)
{
	t_mgr			*manager;
	t_render_view	*view;

	manager = &render->manager;
	view = &manager->render_view;
	view_setup(view, render->width, render->height);
	manager->render_in_progress = 1;
	manager->tileset.tiles_done = 0;
	bitmap_zero(&manager->tileset.tile_state);
	manager->render_view.frame_seq++;
	manager->tileset.tiles_ready = 0;
	manager->render_view.mode = NORMAL_MODE;
}

int	manager_read_mailbox(t_render *render)
{
	t_mgr				*manager;
	t_render_view		*view;
	t_display_mailbox	*mailbox;

	manager = &render->manager;
	view = &manager->render_view;
	mailbox = &render->mailbox;
	if (atomic_load(&mailbox->request_ready) == 0)
		return (0);
	copy_request(view, &mailbox->cam);
	view->frame_seq = mailbox->req_job_id;
	atomic_store(&mailbox->request_ready, 0);
	apply_cam_request(render);
	return (1);
}
