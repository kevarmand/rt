/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:39:22 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 22:40:57 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"

void	interpolate_tile_blocks(t_tile *tile,
			int *last_row, int *last_col);

static t_vec3f	trace_sample_fast(t_data *data, const t_render_view *view,
			int img_x, int img_y)
{
	t_ray			ray;
	t_shading_ctx	ctx;
	t_vec3f			color_out;

	ctx.depth = 0;
	ctx.contribution = 1.0f;
	ctx.current_ior = 1.0f;
	build_ray_for_pixel(view, (float)img_x, (float)img_y, &ray);
	shading_ray_fast(&data->scene, &ray, &ctx, &color_out);
	return (color_out);
}

static void	fast_sample_tile(t_data *data,
			const t_render_view *view, t_tile *tile)
{
	t_vec3f	color;
	int		local_y;
	int		local_x;
	int		index;

	local_y = 0;
	while (local_y < tile->pixel_height)
	{
		local_x = 0;
		while (local_x < tile->pixel_width)
		{
			color = trace_sample_fast(data, view,
					(float)(tile->pixel_x + local_x),
					(float)(tile->pixel_y + local_y));
			index = local_y * tile->pixel_width + local_x;
			tile->hdr_pixels[index] = color;
			local_x += 3;
		}
		local_y += 3;
	}
}

static void	replicate_rows_from(int row, t_tile *tile)
{
	int	y;
	int	x;
	int	w;
	int	src;
	int	dst;

	w = tile->pixel_width;
	if (row < 0 || row >= tile->pixel_height - 1)
		return ;
	y = row + 1;
	while (y < tile->pixel_height)
	{
		x = 0;
		while (x < w)
		{
			src = row * tile->pixel_width + x;
			dst = y * tile->pixel_width + x;
			tile->hdr_pixels[dst] = tile->hdr_pixels[src];
			x++;
		}
		y++;
	}
}

static void	replicate_cols_from(int col, t_tile *tile)
{
	int	x;
	int	y;
	int	w;
	int	src;
	int	dst;

	w = tile->pixel_width;
	if (col < 0 || col >= w - 1)
		return ;
	x = col + 1;
	while (x < w)
	{
		y = 0;
		while (y < tile->pixel_height)
		{
			src = y * w + col;
			dst = y * w + x;
			tile->hdr_pixels[dst] = tile->hdr_pixels[src];
			y++;
		}
		x++;
	}
}

int	render_tile_fast(t_data *data, t_tile *tile, const t_render_view *view)
{
	int	last_row;
	int	last_col;

	fast_sample_tile(data, view, tile);
	interpolate_tile_blocks(tile, &last_row, &last_col);
	replicate_rows_from(last_row, tile);
	replicate_cols_from(last_col, tile);
	return (SUCCESS);
}
