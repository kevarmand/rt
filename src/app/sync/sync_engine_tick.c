/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_engine_tick.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 00:00:00 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 18:51:18 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sync.h"

void	display_invalidate_frame(t_display *display,
		t_display_mailbox *mailbox, int cam)
{
	t_frame	*frame;
	int		seq;

	frame = &display->frame[cam];
	seq = atomic_exchange(&mailbox->expected_seq[cam], 0);
	if (seq && display->ds_sync.in_flight == seq)
		display->ds_sync.in_flight = 0;
	frame->state = FRAME_DIRTY;
	frame->quality = QUALITY_NONE;
	frame->tiles_received = 0;
	display->flag_img_window = 1;
}

static void	apply_invalidation(t_display *display,
		t_display_mailbox *mailbox)
{
	int	cam;

	if (display->flag_render_config_changed)
	{
		cam = 0;
		while (cam < display->total_cams)
			display_invalidate_frame(display, mailbox, cam++);
	}
	else if (display->flag_camera_changed)
		display_invalidate_frame(display, mailbox, display->current_cam);
	display->flag_render_config_changed = 0;
	display->flag_camera_changed = 0;
}

static void	publish_post_settings(t_display *display, t_display_mailbox *mailbox)
{
	unsigned int	settings;

	settings = post_pack(display->render_tonemap, display->post_auto,
			display->post_gamma_step, display->post_ev_step);
	if ((display->post_word & POST_SETTINGS_MASK) != settings)
		display->post_word = ((display->post_word + POST_GENERATION_STEP)
				& ~POST_SETTINGS_MASK) | settings;
	atomic_store(&mailbox->active_cam, display->current_cam);
	atomic_store(&mailbox->post_settings, display->post_word);
}

void	sync_engine_tick(t_data *data)
{
	t_display			*display;
	t_display_mailbox	*mailbox;

	display = &data->display;
	mailbox = &data->engine.render.mailbox;
	apply_invalidation(display, mailbox);
	publish_post_settings(display, mailbox);
	display_engine_receive(display, mailbox);
	display_request_policy(&data->scene, display);
	display_engine_send(&data->scene, display, mailbox);
}
