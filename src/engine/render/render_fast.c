/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:39:22 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/11 11:49:59 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"


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

int shading_ray_fast(const t_scene *scene, const t_ray *ray, 
	t_shading_ctx *ctx, t_vec3f *color_out)
{
	t_hit	hit;

	reset_hit(&hit);
	if (scene_hit(scene, ray, &hit))
	{
		if (hit.kind == HIT_PLANE)
			hit.surface_id = scene->planes[hit.primitive_id].surface_id;
		else if (hit.kind == HIT_PRIMITIVE)
			hit.surface_id = scene->primitives[hit.primitive_id].surface_id;
		*color_out = scene->surfaces[hit.surface_id].color;
	}
	else
	{
		sky_color(ray->dir, color_out);
	}
	return (0);
}

typedef struct s_coef
{
	t_vec3f	c00;
	t_vec3f	c01;
	t_vec3f	c10;
	t_vec3f	c11;
}	t_coef;

void	init_coef(t_coef *coef, t_tile *tile, int x, int y)
{
	coef->c00 = tile->hdr_pixels[(y * tile->pixel_width) + x];
	coef->c01 = tile->hdr_pixels[(y * tile->pixel_width) + (x + 3)];
	coef->c10 = tile->hdr_pixels[((y + 3) * tile->pixel_width) + x];
	coef->c11 = tile->hdr_pixels[((y + 3) * tile->pixel_width) + (x + 3)];
}

static void	bilinear_color(const t_coef *coef, int i, int j, t_vec3f *out)
{
	t_vec3f	cx0;
	t_vec3f	cx1;
	float	tx;
	float	ty;

	tx = (float)i / 3.0f;
	ty = (float)j / 3.0f;
	cx0 = vec3f_add(vec3f_scale(coef->c00, 1.0f - tx),
			vec3f_scale(coef->c01, tx));
	cx1 = vec3f_add(vec3f_scale(coef->c10, 1.0f - tx),
			vec3f_scale(coef->c11, tx));
	*out = vec3f_add(vec3f_scale(cx0, 1.0f - ty),
			vec3f_scale(cx1, ty));
}

static void	interpolate_block(t_tile *tile, int x, int y)
{
	t_coef	coef;
	int		i;
	int		j;
	t_vec3f	color;

	init_coef(&coef, tile, x, y);
	j = 0;
	while (j < 3)
	{
		i = 0;
		while (i < 3)
		{
			bilinear_color(&coef, i, j, &color);
			tile->hdr_pixels[(y + j) * tile->pixel_width + (x + i)] = color;
			i++;
		}
		j++;
	}
}

static void	interpolate_tile_blocks(t_tile *tile,
			int *last_row, int *last_col)
{
	int	y;
	int	x;

	*last_row = -1;
	*last_col = -1;
	y = 0;
	while (y + 3 < tile->pixel_height)
	{
		x = 0;
		while (x + 3 < tile->pixel_width)
		{
			interpolate_block(tile, x, y);
			*last_col = x + 2;
			x += 3;
		}
		*last_row = y + 2;
		y += 3;
	}
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

static void	fill_fast_borders(t_tile *tile, int last_row, int last_col)
{
	replicate_rows_from(last_row, tile);
	replicate_cols_from(last_col, tile);
}


int	render_tile_fast(t_data *data, t_tile *tile, const t_render_view *view)
{
	int	last_row;
	int	last_col;

	fast_sample_tile(data, view, tile);
	interpolate_tile_blocks(tile, &last_row, &last_col);
	fill_fast_borders(tile, last_row, last_col);
	return (SUCCESS);
}
