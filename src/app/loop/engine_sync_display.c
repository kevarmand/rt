/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine_sync_display.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 00:00:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 21:39:22 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_rt.h"
#include "display.h"
#include "engine.h"
#include "render.h"
#include "libft.h"
#include <stdatomic.h>

/* -------------------------------------------------------------------------- */
/* Helpers qualité                                                            */
/* -------------------------------------------------------------------------- */



/*
** Caméra active :
** - AUTO : NONE -> FAST -> NORMAL, puis stop.
** - LOCK_* : vise directement le niveau demandé.
** Retourne QUALITY_NONE si aucun rendu n'est nécessaire.
*/

static t_quality	get_active_target_quality(t_quality current_quality,
					t_user_render_mode user_mode)
{
	if (user_mode == USER_RENDER_AUTO)
	{
		if (current_quality < QUALITY_FAST)
			return (QUALITY_FAST);
		if (current_quality < QUALITY_NORMAL)
			return (QUALITY_NORMAL);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_FAST)
	{
		if (current_quality < QUALITY_FAST)
			return (QUALITY_FAST);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_NORMAL)
	{
		if (current_quality < QUALITY_NORMAL)
			return (QUALITY_NORMAL);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_SUPER)
	{
		if (current_quality < QUALITY_SUPER)
			return (QUALITY_SUPER);
		return (QUALITY_NONE);
	}
	return (QUALITY_NONE);
}

/*
** Background :
** - AUTO : vise NORMAL si on est en dessous.
** - LOCK_* : vise le même niveau que la cam active.
*/

static t_quality	get_background_target_quality(t_quality current_quality,
					t_user_render_mode user_mode)
{
	if (user_mode == USER_RENDER_AUTO)
	{
		if (current_quality < QUALITY_NORMAL)
			return (QUALITY_NORMAL);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_FAST)
	{
		if (current_quality < QUALITY_FAST)
			return (QUALITY_FAST);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_NORMAL)
	{
		if (current_quality < QUALITY_NORMAL)
			return (QUALITY_NORMAL);
		return (QUALITY_NONE);
	}
	if (user_mode == USER_RENDER_LOCK_SUPER)
	{
		if (current_quality < QUALITY_SUPER)
			return (QUALITY_SUPER);
		return (QUALITY_NONE);
	}
	return (QUALITY_NONE);
}

static int	select_background_job(t_display *display,
				int *out_cam_index,
				t_quality *out_target_quality)
{
	int			camera_index;
	t_quality	current_quality;
	t_quality	target_quality;

	camera_index = 0;
	while (camera_index < display->total_cams)
	{
		if (camera_index != display->current_cam)
		{
			current_quality = display->frame[camera_index].quality;
			target_quality = get_background_target_quality(
					current_quality, display->user_render_mode);
			if (target_quality != QUALITY_NONE
				&& target_quality > current_quality)
			{
				*out_cam_index = camera_index;
				*out_target_quality = target_quality;
				return (1);
			}
		}
		camera_index++;
	}
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Receive : manager -> display                                              */
/* -------------------------------------------------------------------------- */

static void	display_engine_receive(t_display *display,
				t_display_mailbox *mailbox)
{
	int	camera_id;
	int	is_full_frame;
	int	snapshot_job_id;
	int	snapshot_mode;

	if (atomic_load(&mailbox->snapshot_ready) == 0)
		return ;
	//debug icic ausis on affiche la reception
	printf("Display receives snapshot: cam %d job_id %d mode %d tiles_done %d/%d\n",
		mailbox->snap_camera_id,
		mailbox->snap_job_id,
		mailbox->snapshot_render_mode,
		mailbox->tiles_done,
		mailbox->tile_count);
	camera_id = mailbox->snap_camera_id;
	snapshot_job_id = mailbox->snap_job_id;
	snapshot_mode = mailbox->snapshot_render_mode;
	is_full_frame = (mailbox->tiles_done >= mailbox->tile_count);
	if (is_full_frame || display->current_cam == camera_id)
	{
		ft_memcpy(display->frame[camera_id].rgb_pixels,
			mailbox->rgb_pixels,
			(size_t)display->pixel_count * sizeof(int));
		if (is_full_frame)
			display->frame[camera_id].quality
				= snapshot_mode;
		if (display->current_cam == camera_id)
			display->flag_img_buffer = 1;
	}
	if (is_full_frame && mailbox->req_job_id == snapshot_job_id)
		display->ds_sync.in_flight = 0;
	atomic_store(&mailbox->snapshot_ready, 0);
}

/* -------------------------------------------------------------------------- */
/* Policy : décide quoi rendre ensuite (sans atomics, sans mailbox)          */
/* -------------------------------------------------------------------------- */

void	display_request_policy(t_scene *scene, t_display *display)
{
	t_ds_sync	*sync;
	t_quality	current_quality;
	t_quality	target_quality;
	int			background_cam_index;

	(void)scene;
	sync = &display->ds_sync;
	if (display->flag_camera_changed)
	{
		t_user_render_mode	mode;
		t_quality			target;

		mode = display->user_render_mode;
		display->flag_camera_changed = 0;
		if (mode == USER_RENDER_AUTO)
			target = QUALITY_FAST;
		else
			target = (t_quality)mode;
		sync->req_cam = display->current_cam;
		sync->req_mode = target;
		sync->pending = 1;
		return ;
	}
	if (sync->in_flight || sync->pending)
	{
		// if (sync->in_flight)
		// 	printf("f");
		// if (sync->pending)
		// 	printf("p");
		// printf("\n");
		return ;
	}
	current_quality = display->frame[display->current_cam].quality;
	target_quality = get_active_target_quality(current_quality,
			display->user_render_mode);
	if (target_quality != QUALITY_NONE
		&& target_quality > current_quality)
	{
		sync->req_cam = display->current_cam;
		sync->req_mode = target_quality;
		sync->pending = 1;
		return ;
	}
	if (select_background_job(display, &background_cam_index,
			&target_quality))
	{
		sync->req_cam = background_cam_index;
		sync->req_mode = target_quality;
		sync->pending = 1;
	}
}

/* -------------------------------------------------------------------------- */
/* Send : display -> manager                                                 */
/* -------------------------------------------------------------------------- */

static void	display_engine_send(t_scene *scene, t_display *display,
				t_display_mailbox *mailbox)
{
	t_ds_sync	*sync;
	int			request_ready;
	int			camera_index;

	sync = &display->ds_sync;
	if (!sync->pending)
		return ;
	request_ready = atomic_load(&mailbox->request_ready);
	if (request_ready != 0)
		return ;
	camera_index = sync->req_cam;
	mailbox->cam = scene->cameras[camera_index];
	mailbox->request_camera_id = camera_index;
	mailbox->request_render_mode = sync->req_mode;
	mailbox->req_job_id++;
	//petit debug on affiche la requete :
	printf("Display requests render: cam %d mode %d [job_id %d]\n",
		mailbox->request_camera_id,
		mailbox->request_render_mode,
		mailbox->req_job_id);
	atomic_store(&mailbox->request_ready, 1);
	sync->in_flight = 1;
	sync->pending = 0;
}

/* -------------------------------------------------------------------------- */
/* Master                                                                    */
/* -------------------------------------------------------------------------- */

void	engine_sync_display(t_data *data)
{
	t_display			*display;
	t_display_mailbox	*mailbox;

	display = &data->display;
	mailbox = &data->engine.render.mailbox;
	display_engine_receive(display, mailbox);
	display_request_policy(&data->scene, display);
	display_engine_send(&data->scene, display, mailbox);
}
