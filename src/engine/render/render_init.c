/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:46:18 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 22:27:39 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "engine.h"
#include "vector.h"
#include "rt_config.h"

static void	render_init_struct(t_render *render, int width, int height)
{
	memset(render, 0, sizeof(t_render));
	render->width = width;
	render->height = height;
	atomic_store(&render->cancel_flag, 0);
}

/* ---------- Helper 2 : allocate HDR + RGB main buffers ---------- */

static int	render_init_main_buffers(t_render *render)
{
	int		total_pixels;
	size_t	bytes_hdr;
	size_t	bytes_rgb;

	total_pixels = render->width * render->height;
	if (total_pixels <= 0)
		return (ERR_INTERNAL);
	bytes_hdr = (size_t)total_pixels * sizeof(t_vec3f);
	bytes_rgb = (size_t)total_pixels * sizeof(int);
	render->manager.hdr_buffer = malloc(bytes_hdr);
	if (!render->manager.hdr_buffer)
		return (ERR_MALLOC);
	memset(render->manager.hdr_buffer, 0, bytes_hdr);
	render->manager.rgb_buffer = malloc(bytes_rgb);
	if (!render->manager.rgb_buffer)
		return (ERR_MALLOC);
	memset(render->manager.rgb_buffer, 0, bytes_rgb);
	return (SUCCESS);
}

/* ---------- Helper 3 : init mailbox buffers ---------- */

static int	render_init_mailbox(t_render *render)
{
	int		total_pixels;
	size_t	bytes_rgb;

	total_pixels = render->width * render->height;
	bytes_rgb = (size_t)total_pixels * sizeof(int);
	render->mailbox.rgb_pixels = malloc(bytes_rgb);
	if (!render->mailbox.rgb_pixels)
		return (ERR_MALLOC);
	memset(render->mailbox.rgb_pixels, 0, bytes_rgb);
	atomic_store(&render->mailbox.snapshot_ready, 0);
	atomic_store(&render->mailbox.request_ready, 0);
	return (SUCCESS);
}

/* ---------- Helper 4 : allocate worker array ---------- */

static int render_compute_worker_count(void)
{
	if (MAX_WORKER_THREADS < 1)
		return (0);
	return (MAX_WORKER_THREADS);
}

static int	render_init_workers(t_render *render)
{
	int		worker_count;
	size_t	bytes;

	worker_count = render_compute_worker_count();
	if (worker_count <= 0)
		return (SUCCESS);
	render->workers.count = worker_count;
	bytes = sizeof(t_worker) * (size_t)worker_count;
	render->workers.array = malloc(bytes);
	if (!render->workers.array)
		return (ERR_MALLOC);
	memset(render->workers.array, 0, bytes);
	return (SUCCESS);
}

/* ---------- Helper 5 : allocate worker tile buffers ---------- */

static int	render_init_worker_tiles(t_render *render)
{
	int		index;
	size_t	tile_pixels;
	size_t	bytes;

	tile_pixels = (size_t)TILE_SIZE * (size_t)TILE_SIZE;
	bytes = tile_pixels * sizeof(t_vec3f);
	index = 0;
	while (index < render->workers.count)
	{
		render->workers.array[index].tile.hdr_pixels = malloc(bytes);
		if (!render->workers.array[index].tile.hdr_pixels)
			return (ERR_MALLOC);
		memset(render->workers.array[index].tile.hdr_pixels, 0, bytes);
		atomic_store(&render->workers.array[index].worker_state, WORKER_IDLE);
		index++;
	}
	return (SUCCESS);
}

/* ---------- Helper 6 : init tileset ---------- */

static int	render_init_tileset(t_render *render)
{
	t_tileset	*tileset;

	tileset = &render->manager.tileset;
	tileset->tile_width = TILE_SIZE;
	tileset->tile_height = TILE_SIZE;
	tileset->tiles_per_row = (render->width + TILE_SIZE - 1) / TILE_SIZE;
	tileset->tiles_per_col = (render->height + TILE_SIZE - 1) / TILE_SIZE;
	tileset->tiles_total = tileset->tiles_per_row * tileset->tiles_per_col;
	if (tileset->tiles_total <= 0)
		return (ERR_INTERNAL);
	if (bitmap_init(&tileset->tile_state, tileset->tiles_total) != SUCCESS)
		return (ERR_MALLOC);

	tileset->tiles_active = 0;
	tileset->tiles_done = 0;
	return (SUCCESS);
}

int	render_init(t_render *render, int width, int height)
{
	int	status;

	render_init_struct(render, width, height);
	status = render_init_main_buffers(render);
	if (status != SUCCESS)
		return (status);
	status = render_init_mailbox(render);
	if (status != SUCCESS)
		return (status);
	status = render_init_workers(render);
	if (status != SUCCESS)
		return (status);
	status = render_init_worker_tiles(render);
	if (status != SUCCESS)
		return (status);
	status = render_init_tileset(render);
	if (status != SUCCESS)
		return (status);
	render->manager.render_in_progress = 0;
	render->manager.thread_id = 0;
	return (SUCCESS);
}
