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
#include <time.h>

#define PREVIEW_INTERVAL_NS 66000000ull

typedef struct s_publication
{
	const t_vec3f	*pixels;
	t_render_view	view;
	float			exposure;
	int				cam;
	int				done;
	int				is_render;
}	t_publication;

static uint64_t	now_ns(void)
{
	struct timespec	ts;

	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
		return (0);
	return ((uint64_t)ts.tv_sec * 1000000000ull + (uint64_t)ts.tv_nsec);
}

static void	publish(t_render *render, const t_publication *pub)
{
	t_mgr				*mgr;
	t_display_mailbox	*mailbox;

	mgr = &render->manager;
	mailbox = &render->mailbox;
	manager_convert_image(mgr, pub->pixels, mailbox->rgb_pixels,
		(size_t)render->width * render->height, pub->exposure, pub->view.mode);
	mailbox->tile_count = mgr->tileset.tiles_total;
	mailbox->tiles_done = pub->done;
	mailbox->snap_job_id = pub->view.frame_seq;
	mailbox->snap_camera_id = pub->cam;
	mailbox->snapshot_render_mode = pub->view.mode;
	mailbox->snap_post_settings = mgr->post_settings;
	mailbox->snap_is_render = pub->is_render;
	atomic_store(&mailbox->snapshot_ready, 1);
}

void	manager_cache_completed(t_render *render)
{
	t_mgr		*mgr;
	t_hdr_frame	*frame;
	t_vec3f		*previous;

	mgr = &render->manager;
	frame = &mgr->frames[mgr->current_cam_id];
	frame->auto_exposure = manager_auto_exposure(mgr,
			render->width, render->height);
	previous = frame->pixels;
	frame->pixels = mgr->hdr_buffer;
	mgr->hdr_buffer = previous;
	frame->view = mgr->render_view;
	frame->published_settings = 0;
	frame->completion_pending = 1;
	mgr->render_in_progress = 0;
}

static int	publish_cached(t_render *render, int cam)
{
	t_mgr			*mgr;
	t_hdr_frame		*frame;
	t_publication	pub;

	mgr = &render->manager;
	frame = &mgr->frames[cam];
	if (!frame->view.mode || (!frame->completion_pending
			&& frame->published_settings == mgr->post_settings))
		return (0);
	pub = (t_publication){frame->pixels, frame->view, frame->auto_exposure,
		cam, mgr->tileset.tiles_total, frame->completion_pending};
	publish(render, &pub);
	frame->published_settings = mgr->post_settings;
	frame->completion_pending = 0;
	return (1);
}

static int	publish_preview(t_render *render)
{
	t_mgr			*mgr;
	t_publication	pub;
	uint64_t		now;

	mgr = &render->manager;
	if (mgr->render_view.mode == QUALITY_FAST || mgr->tileset.tiles_done == 0)
		return (0);
	if (mgr->preview_tiles == mgr->tileset.tiles_done
		&& mgr->preview_settings == mgr->post_settings)
		return (0);
	now = now_ns();
	if (mgr->preview_settings == mgr->post_settings && now
		&& mgr->preview_time_ns
		&& now - mgr->preview_time_ns < PREVIEW_INTERVAL_NS)
		return (0);
	pub = (t_publication){mgr->hdr_buffer, mgr->render_view,
		mgr->preview_exposure, mgr->current_cam_id, mgr->tileset.tiles_done, 1};
	publish(render, &pub);
	mgr->preview_tiles = mgr->tileset.tiles_done;
	mgr->preview_settings = mgr->post_settings;
	mgr->preview_time_ns = now;
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
		&& mgr->tileset.tiles_done > 0)
	{
		if (publish_preview(render))
			return (1);
	}
	else if (publish_cached(render, mgr->active_cam))
		return (1);
	cam = 0;
	while (cam < mgr->camera_count)
	{
		if (cam != mgr->active_cam && mgr->frames[cam].completion_pending
			&& publish_cached(render, cam))
			return (1);
		cam++;
	}
	cam = 0;
	while (cam < mgr->camera_count)
	{
		if (cam != mgr->active_cam && publish_cached(render, cam))
			return (1);
		cam++;
	}
	return (0);
}
