/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_worker_tile.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 17:57:57 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/05 22:12:06 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static int	clamped_tile_size(int start, int tile_size, int image_size)
{
	int	remaining;

	remaining = image_size - start;
	if (remaining < tile_size)
		return (remaining);
	return (tile_size);
}

int	init_worker_tile(t_tileset *tileset, t_tile *tile,
			int image_width, int image_height)
{
	int	row;
	int	col;
	int	start_x;
	int	start_y;
	int	tile_id;

	tile_id = manager_dispatch_tile(tileset);
	if (tile_id < 0)
		return (0);
	tile->tile_id = tile_id;
	col = tile_id % tileset->tiles_per_row;
	row = tile_id / tileset->tiles_per_row;
	start_x = col * tileset->tile_width;
	start_y = row * tileset->tile_height;
	tile->pixel_x = start_x;
	tile->pixel_y = start_y;
	tile->pixel_width = clamped_tile_size(start_x,
			tileset->tile_width, image_width);
	tile->pixel_height = clamped_tile_size(start_y,
			tileset->tile_height, image_height);
	return (1);
}
