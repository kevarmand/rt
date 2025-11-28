/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_hdr_to_rgb.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 18:18:51 by kearmand          #+#    #+#             */
/*   Updated: 2025/11/28 16:33:20 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include "color.h"
#include "render.h"

typedef struct s_tile_rect
{
	int	x;
	int	y;
	int	width;
	int	height;
}	t_tile_rect;

static void	compute_tile_rect(t_render *render,
			int tile_id, t_tile_rect *rect)
{
	t_mgr		*manager;
	t_tileset	*tileset;
	int			col;
	int			row;

	manager = &render->manager;
	tileset = &manager->tileset;
	col = tile_id % tileset->tiles_per_row;
	row = tile_id / tileset->tiles_per_row;
	rect->x = col * tileset->tile_width;
	rect->y = row * tileset->tile_height;
	rect->width = tileset->tile_width;
	if (rect->x + rect->width > render->width)
		rect->width = render->width - rect->x;
	rect->height = tileset->tile_height;
	if (rect->y + rect->height > render->height)
		rect->height = render->height - rect->y;
}

static void	convert_tile_row(t_mgr *manager,
			int image_width, t_tile_rect *rect, int local_y)
{
	t_vec3f	color_lin;
	int		rgb_value;
	int		rgb[3];
	int		local_x;
	int		hdr_index;

	local_x = 0;
	while (local_x < rect->width)
	{
		hdr_index = (rect->y + local_y) * image_width
			+ (rect->x + local_x);
		color_lin = manager->hdr_buffer[hdr_index];
		linear_to_rgb8_vec(&color_lin, rgb);
		rgb_value = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
		manager->rgb_buffer[hdr_index] = rgb_value;
		local_x++;
	}
}

static void	convert_tile_hdr_to_rgb(t_render *render, int tile_id)
{
	t_mgr		*manager;
	t_tile_rect	rect;
	int			local_y;

	manager = &render->manager;
	compute_tile_rect(render, tile_id, &rect);
	local_y = 0;
	while (local_y < rect.height)
	{
		convert_tile_row(manager, render->width, &rect, local_y);
		local_y++;
	}
}
#include <stdio.h>

static int	find_next_tile_to_convert(t_tileset *tileset)
{
	int	tile_id;

	tile_id = 0;
	while (tile_id < tileset->tiles_total)
	{
		if (bitmap_get(&tileset->tile_state, tile_id) != 0)
			return (tile_id);
		tile_id++;
	}
	return (-1);
}

int	manager_convert_hdr_to_rgb(t_render *render)
{
	int tile_id;

 	tile_id = find_next_tile_to_convert(&render->manager.tileset);
	if (tile_id < 0)
		return (0);
	convert_tile_hdr_to_rgb(render, tile_id);
	bitmap_clear(&render->manager.tileset.tile_state, tile_id);
	render->manager.tileset.tiles_done++;
	render->manager.tileset.tiles_ready--;
	return (1);
}
