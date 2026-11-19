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

static void	copy_request(t_render_view *view, const t_camera *src)
{
	view->origin = src->origin;
	view->forward = src->forward;
	view->right = src->right;
	view->fov_deg = src->fov_deg;
}

int	manager_read_mailbox(t_render *render)
{
	t_mgr				*manager;
	t_render_view		*view;
	t_display_mailbox	*mailbox;
	t_hdr_frame			*frame;

	manager = &render->manager;
	view = &manager->render_view;
	mailbox = &render->mailbox;
	if (!atomic_load(&mailbox->request_ready))
		return (0);
	if (mailbox->req_job_id != atomic_load(
			&mailbox->expected_seq[mailbox->request_camera_id]))
	{
		atomic_store(&mailbox->request_ready, 0);
		return (1);
	}
	copy_request(view, &mailbox->cam);
	view->frame_seq = mailbox->req_job_id;
	view->mode = mailbox->request_render_mode;
	view->ssaa = mailbox->request_ssaa;
	manager->current_cam_id = mailbox->request_camera_id;
	frame = &manager->frames[manager->current_cam_id];
	frame->job_id = view->frame_seq;
	frame->mode = view->mode;
	frame->done = 0;
	manager->tileset.assigned = 0;
	manager->tileset.cursor = &frame->cursor;
	manager->render_in_progress = 1;
	atomic_store(&mailbox->request_ready, 0);
	view_setup(view, render->width, render->height);
	return (1);
}
