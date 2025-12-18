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
#include <stdatomic.h>
#include <string.h>
#include <stdatomic.h>
#include "vector.h"
#include "bitmap.h"
#include "libft.h"
#include <stdio.h>

static void	manager_copy_tile_pixels(t_render *render, t_tile *tile)
{
	t_mgr		*mgr;
	int			row;
	int			img_width;
	t_vec3f		*dst;
	t_vec3f		*src;

	mgr = &render->manager;
	img_width = render->width;
	row = 0;
	while (row < tile->pixel_height)
	{
		dst = mgr->hdr_buffer
			+ (tile->pixel_y + row) * img_width + tile->pixel_x;
		src = tile->hdr_pixels + row * tile->pixel_width;
		ft_memcpy(dst, src,
			(size_t)tile->pixel_width * sizeof(t_vec3f));
		row++;
	}
}

static void	manager_handle_completed_tile(t_render *render, t_worker *worker)
{
	t_mgr	*mgr;
	t_tile	*tile;

	mgr = &render->manager;
	mgr->tileset.tiles_active--;
	if (worker->local_view.frame_seq != mgr->render_view.frame_seq)
	{
		atomic_store(&worker->worker_state, WORKER_IDLE);
		return ;
	}
	tile = &worker->tile;
	manager_copy_tile_pixels(render, tile);
	bitmap_set(&mgr->tileset.tile_state, tile->tile_id);
	mgr->tileset.tiles_ready++;
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
