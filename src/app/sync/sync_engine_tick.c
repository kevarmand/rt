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
	publish_post_settings(display, mailbox);
	display_engine_receive(display, mailbox);
	display_request_policy(&data->scene, display);
	display_engine_send(&data->scene, display, mailbox);
}
