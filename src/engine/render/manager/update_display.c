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
#include <math.h>

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

void	manager_finish_render(t_render *render)
{
	t_mgr		*mgr;
	t_hdr_frame	*frame;

	mgr = &render->manager;
	frame = &mgr->frames[mgr->current_cam_id];
	mgr->render_in_progress = 0;
	if (frame->job_id == atomic_load(&render->mailbox.expected_seq[mgr->current_cam_id]))
		frame->auto_ev = auto_ev_step(manager_auto_exposure(mgr,
					render->width, render->height));
}

static int	needs_publication(t_mgr *mgr, int cam)
{
	t_hdr_frame	*frame;

	frame = &mgr->frames[cam];
	return (frame->revision && (frame->revision != frame->published_revision
			|| frame->published_settings != mgr->post_settings));
}

static int	select_camera(t_mgr *mgr)
{
	int	cam;

	if (needs_publication(mgr, mgr->active_cam))
		return (mgr->active_cam);
	if (mgr->render_in_progress && mgr->current_cam_id == mgr->active_cam)
		return (-1);
	cam = 0;
	while (cam < mgr->camera_count)
	{
		if (needs_publication(mgr, cam))
			return (cam);
		cam++;
	}
	return (-1);
}

static void	begin_pass(t_mgr *mgr, t_rgb_image *image, int cam)
{
	t_hdr_frame	*frame;

	frame = &mgr->frames[cam];
	mgr->pass = (t_rgb_pass){.camera_id = cam, .job_id = frame->job_id,
		.mode = frame->mode, .done = frame->done, .auto_ev = frame->auto_ev,
		.revision = frame->revision};
	if (image->camera_id != cam || image->settings != mgr->post_settings)
		ft_memset(image->tile_seq, -1, mgr->tileset.tiles_total * sizeof(int));
	image->camera_id = cam;
	image->settings = mgr->post_settings;
}

static void	publish(t_render *render, t_rgb_image *image)
{
	t_mgr				*mgr;
	t_display_mailbox	*box;
	t_hdr_frame			*frame;

	mgr = &render->manager;
	box = &render->mailbox;
	frame = &mgr->frames[mgr->pass.camera_id];
	box->snap_camera_id = mgr->pass.camera_id;
	box->snap_job_id = mgr->pass.job_id;
	box->snapshot_render_mode = mgr->pass.mode;
	box->tile_count = mgr->tileset.tiles_total;
	box->tiles_done = mgr->pass.done;
	box->snap_auto_ev = mgr->pass.auto_ev;
	box->snap_post_settings = image->settings;
	frame->published_settings = image->settings;
	frame->published_revision = mgr->pass.revision;
	mgr->pass.camera_id = -1;
	/* Release the completed image. Only the display may use image_id
	 * until its acknowledgement returns a writable image. */
	atomic_store(&box->snapshot_ready, 1);
}

int	manager_update_display(t_render *render)
{
	t_mgr		*mgr;
	t_rgb_image	*image;
	int			cam;

	if (atomic_load(&render->mailbox.snapshot_ready))
		return (0);
	mgr = &render->manager;
	cam = select_camera(mgr);
	if (cam < 0)
		return (0);
	image = &mgr->images[render->mailbox.image_id];
	if (cam != mgr->pass.camera_id)
		begin_pass(mgr, image, cam);
	if (manager_convert_tiles(render, image))
		publish(render, image);
	return (1);
}
