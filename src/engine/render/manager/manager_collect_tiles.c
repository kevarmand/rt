/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_collect_tiles.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:26:47 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:31:45 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include <string.h>

static void	manager_copy_tile_pixels(t_hdr_frame *frame, int img_width, t_tile *tile)
{
	int			row;
	t_vec3f		*dst;
	t_vec3f		*src;

	row = 0;
	while (row < tile->pixel_height)
	{
		dst = frame->pixels
			+ (tile->pixel_y + row) * img_width + tile->pixel_x;
		src = tile->hdr_pixels + row * tile->pixel_width;
		memcpy(dst, src,
			(size_t)tile->pixel_width * sizeof(t_vec3f));
		row++;
	}
}

static void	manager_handle_completed_tile(t_render *render, t_worker *worker)
{
	t_hdr_frame	*frame;
	int			seq;
	int			id;

	frame = &render->manager.frames[worker->camera_id];
	seq = worker->local_view.frame_seq;
	id = worker->tile.tile_id;
	if (seq > frame->tile_seq[id])
	{
		manager_copy_tile_pixels(frame, render->width, &worker->tile);
		frame->tile_seq[id] = seq;
		frame->revision++;
		if (seq == frame->job_id)
			frame->done++;
	}
	atomic_store(&worker->worker_state, WORKER_IDLE);
}

int	manager_collect_tiles(t_render *render)
{
	t_workers	*workers;
	int			worker_index;
	int			did_work;

	workers = &render->workers;
	worker_index = 0;
	did_work = 0;
	while (worker_index < workers->count)
	{
		if (atomic_load(&workers->array[worker_index].worker_state)
			== WORKER_DONE)
		{
			manager_handle_completed_tile(render,
				&workers->array[worker_index]);
			did_work = 1;
		}
		worker_index++;
	}
	return (did_work);
}
