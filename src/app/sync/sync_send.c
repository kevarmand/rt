/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_send.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 18:29:44 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/16 20:10:48 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sync.h"

void	display_engine_send(t_scene *scene, t_display *display,
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
	ft_printf("Display requests render: cam %d mode %d [job_id %d]\n",
		mailbox->request_camera_id,
		mailbox->request_render_mode,
		mailbox->req_job_id);
	atomic_store(&mailbox->request_ready, 1);
	sync->in_flight = 1;
	sync->pending = 0;
}
