/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_mailbox.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 18:47:20 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 16:21:32 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include "render.h"
#include "bitmap.h"
#include "libft.h"
#include <stdio.h>

static void	copy_request(t_render_view *view, const t_camera *src)
{
	view->origin = src->origin;
	view->forward = src->forward;
	view->right = src->right;
	view->fov_deg = src->fov_deg;
}

static int	same_vec(t_vec3f a, t_vec3f b)
{
	return (a[0] == b[0] && a[1] == b[1] && a[2] == b[2]);
}

static int	same_view(const t_render_view *a, const t_render_view *b)
{
	return (same_vec(a->origin, b->origin)
		&& same_vec(a->forward, b->forward)
		&& same_vec(a->right, b->right) && a->fov_deg == b->fov_deg);
}

static void	prepare_preview(t_render *render)
{
	t_mgr		*mgr;
	t_hdr_frame	*frame;
	size_t		bytes;

	mgr = &render->manager;
	frame = &mgr->frames[mgr->current_cam_id];
	bytes = (size_t)render->width * render->height * sizeof(t_vec3f);
	mgr->preview_tiles = -1;
	mgr->preview_time_ns = 0;
	mgr->preview_exposure = 1.0f;
	if (frame->view.mode && same_view(&frame->view, &mgr->render_view))
	{
		ft_memcpy(mgr->hdr_buffer, frame->pixels, bytes);
		mgr->preview_exposure = frame->auto_exposure;
	}
	else
		ft_memset(mgr->hdr_buffer, 0, bytes);
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
	manager->tileset.tiles_ready = 0;
	prepare_preview(render);
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
	view->mode = mailbox->request_render_mode;
	view->ssaa = mailbox->request_ssaa;
	manager->current_cam_id = mailbox->request_camera_id;
	atomic_store(&mailbox->request_ready, 0);
	apply_cam_request(render);
	return (1);
}
