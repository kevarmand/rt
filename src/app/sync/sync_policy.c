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

/* Only a READY frame can satisfy a quality request. */
static t_quality	frame_quality(const t_frame *frame)
{
	if (frame->state == FRAME_READY)
		return (frame->quality);
	return (QUALITY_NONE);
}

static int	request_frame(t_display *display, int cam, int active)
{
	t_frame		*frame;
	t_quality	quality;
	t_quality	target;

	frame = &display->frame[cam];
	if (frame->state == FRAME_RENDERING)
		return (0);
	quality = frame_quality(frame);
	if (active)
		target = get_active_target_quality(quality, display->user_render_mode);
	else
		target = get_background_target_quality(quality,
				display->user_render_mode);
	if (target == QUALITY_NONE || target <= quality)
		return (0);
	display->ds_sync.req_cam = cam;
	display->ds_sync.req_mode = target;
	display->ds_sync.pending = 1;
	return (1);
}

void	display_request_policy(const t_scene *scene, t_display *display)
{
	int	cam;

	(void)scene;
	/* Pending requests have not been dispatched: re-evaluate every tick. */
	display->ds_sync.pending = 0;
	if (request_frame(display, display->current_cam, 1))
		return ;
	if (display->ds_sync.in_flight)
		return ;
	cam = 0;
	while (cam < display->total_cams)
	{
		if (cam != display->current_cam && request_frame(display, cam, 0))
			return ;
		cam++;
	}
}
