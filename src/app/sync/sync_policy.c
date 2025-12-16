/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_policy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:50:21 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 20:30:34 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sync.h"

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

static int	policy_on_camera_changed(t_display *display, t_ds_sync *sync)
{
	t_user_render_mode	mode;
	t_quality			target;

	if (!display->flag_camera_changed)
		return (0);
	mode = display->user_render_mode;
	display->flag_camera_changed = 0;
	if (mode == USER_RENDER_AUTO)
		target = QUALITY_FAST;
	else
		target = (t_quality)mode;
	sync->req_cam = display->current_cam;
	sync->req_mode = target;
	sync->pending = 1;
	return (1);
}

static int	policy_on_active_upgrade(t_display *display, t_ds_sync *sync)
{
	t_quality	current_quality;
	t_quality	target_quality;

	current_quality = display->frame[display->current_cam].quality;
	target_quality = get_active_target_quality(current_quality,
			display->user_render_mode);
	if (target_quality == QUALITY_NONE || target_quality <= current_quality)
		return (0);
	sync->req_cam = display->current_cam;
	sync->req_mode = target_quality;
	sync->pending = 1;
	return (1);
}

void	display_request_policy(const t_scene *scene, t_display *display)
{
	t_ds_sync	*sync;
	int			background_cam_index;
	t_quality	target_quality;

	(void)scene;
	sync = &display->ds_sync;
	if (policy_on_camera_changed(display, sync))
		return ;
	if (sync->in_flight || sync->pending)
		return ;
	if (policy_on_active_upgrade(display, sync))
		return ;
	if (select_background_job(display, &background_cam_index, &target_quality))
	{
		sync->req_cam = background_cam_index;
		sync->req_mode = target_quality;
		sync->pending = 1;
	}
}
