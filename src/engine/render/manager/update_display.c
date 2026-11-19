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
#include "display.h"
#include <math.h>

typedef struct s_publication
{
	const t_vec3f	*pixels;
	t_render_view	view;
	int				auto_ev;
	int				cam;
	int				done;
}	t_publication;

static void	publish(t_render *render, const t_publication *pub)
{
	t_mgr				*mgr;
	t_display_mailbox	*mailbox;

	mgr = &render->manager;
	mailbox = &render->mailbox;
	manager_convert_image(mgr, pub->pixels, mailbox->rgb_pixels,
		(size_t)render->width * render->height);
	mailbox->tile_count = mgr->tileset.tiles_total;
	mailbox->tiles_done = pub->done;
	mailbox->snap_job_id = pub->view.frame_seq;
	mailbox->snap_camera_id = pub->cam;
	mailbox->snapshot_render_mode = pub->view.mode;
	mailbox->snap_post_settings = mgr->post_settings;
	mailbox->snap_auto_ev = pub->auto_ev;
	atomic_store(&mailbox->snapshot_ready, 1);
}

/* Auto exposure recommends an absolute EV, not a hidden multiplier. */
static int	auto_ev_step(float exposure)
{
	float	ev;

	if (!(exposure > 0.0f))
		return (POST_EV_MIN);
	ev = log2f(exposure) * 10.0f;
	if (ev <= POST_EV_MIN)
		return (POST_EV_MIN);
	if (ev >= POST_EV_MAX)
		return (POST_EV_MAX);
	return ((int)roundf(ev));
}

void	manager_cache_completed(t_render *render)
{
	t_mgr		*mgr;
	t_hdr_frame	*frame;
	t_vec3f		*previous;

	mgr = &render->manager;
	if (mgr->render_view.frame_seq != atomic_load(
			&render->mailbox.expected_seq[mgr->current_cam_id]))
	{
		mgr->render_in_progress = 0;
		return ;
	}
	frame = &mgr->frames[mgr->current_cam_id];
	frame->auto_ev = auto_ev_step(manager_auto_exposure(mgr,
			render->width, render->height));
	previous = frame->pixels;
	frame->pixels = mgr->hdr_buffer;
	mgr->hdr_buffer = previous;
	frame->view = mgr->render_view;
	frame->published_settings = 0;
	mgr->render_in_progress = 0;
}

static int	publish_cached(t_render *render, int cam)
{
	t_mgr			*mgr;
	t_hdr_frame		*frame;
	t_publication	pub;

	mgr = &render->manager;
	frame = &mgr->frames[cam];
	if (!frame->view.frame_seq || frame->view.frame_seq != atomic_load(
			&render->mailbox.expected_seq[cam])
		|| frame->published_settings == mgr->post_settings)
		return (0);
	pub = (t_publication){frame->pixels, frame->view, frame->auto_ev,
		cam, mgr->tileset.tiles_total};
	publish(render, &pub);
	frame->published_settings = mgr->post_settings;
	return (1);
}

static int	publish_preview(t_render *render)
{
	t_mgr			*mgr;
	t_publication	pub;

	mgr = &render->manager;
	if (mgr->render_view.mode == QUALITY_FAST || mgr->tileset.tiles_done == 0
		|| mgr->render_view.frame_seq != atomic_load(
			&render->mailbox.expected_seq[mgr->current_cam_id]))
		return (0);
	if (mgr->preview_tiles == mgr->tileset.tiles_done
		&& mgr->preview_settings == mgr->post_settings)
		return (0);
	pub = (t_publication){mgr->hdr_buffer, mgr->render_view,
		0, mgr->current_cam_id, mgr->tileset.tiles_done};
	publish(render, &pub);
	mgr->preview_tiles = mgr->tileset.tiles_done;
	mgr->preview_settings = mgr->post_settings;
	return (1);
}

int	manager_update_display(t_render *render)
{
	t_mgr	*mgr;
	int		cam;

	mgr = &render->manager;
	if (atomic_load(&render->mailbox.snapshot_ready))
		return (0);
	if (mgr->render_in_progress && mgr->current_cam_id == mgr->active_cam
		&& publish_preview(render))
		return (1);
	if (publish_cached(render, mgr->active_cam))
		return (1);
	cam = 0;
	while (cam < mgr->camera_count)
	{
		if (cam != mgr->active_cam && publish_cached(render, cam))
			return (1);
		cam++;
	}
	return (0);
}
