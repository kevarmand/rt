/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_init2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 22:31:46 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/20 21:43:25 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>
#include "errors.h"
#include "engine.h"
#include "vector.h"
#include "rt_config.h"
#include "bitmap.h"
#include "libft.h"

/* ---------- Helper 4 : allocate worker array ---------- */

static int	render_compute_worker_count(void)
{
	if (MAX_WORKER_THREADS < 1)
		return (0);
	return (MAX_WORKER_THREADS);
}

int	render_init_workers(t_render *render)
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
	ft_memset(render->workers.array, 0, bytes);
	return (SUCCESS);
}

/* ---------- Helper 5 : allocate worker tile buffers ---------- */

int	render_init_worker_tiles(t_render *render)
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
		ft_memset(render->workers.array[index].tile.hdr_pixels, 0, bytes);
		atomic_store(&render->workers.array[index].worker_state, WORKER_IDLE);
		index++;
	}
	return (SUCCESS);
}

/* ---------- Helper 6 : init tileset ---------- */

int	render_init_tileset(t_render *render)
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
