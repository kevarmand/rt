/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 13:39:22 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/10 19:06:09 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "render.h"
#include "scene.h"
#include "rt_config.h"
#include "new_rt.h"
#include "errors.h"


static int	calcul_pixel_color2(t_data *data, const t_render_view *view,
			int img_x, int img_y, t_vec3f *out_color)
{
	t_ray			ray;
	t_shading_ctx	ctx;

	ctx.depth = 0;
	ctx.contribution = 1.0f;
	ctx.current_ior = 1.0f;
	build_ray_for_pixel(view, img_x, img_y, &ray);
	shading_ray_fast(&data->scene, &ray, &ctx, out_color);
	return (SUCCESS);
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


void	interpolate_case(const t_tile *tile, int x, int y)
{
	t_coef	coef;

	init_coef(&coef, tile, x, y);

	//pour chaque pixel entre les 4 coins ( pseudo code)
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			t_vec3f	cx0;
			t_vec3f	cx1;
			t_vec3f	cxy;
			float	tx;
			float	ty;
			tx = (float)i / 3.0f;
			ty = (float)j / 3.0f;
			cx0 = vec3f_add(vec3f_scale(coef.c00, (1.0f - tx)),
					vec3f_scale(coef.c01, tx));
			cx1 = vec3f_add(vec3f_scale(coef.c10, (1.0f - tx)),	
					vec3f_scale(coef.c11, tx));
			cxy = vec3f_add(vec3f_scale(cx0, (1.0f - ty)),
					vec3f_scale(cx1, ty));
			tile->hdr_pixels[((y + j) * tile->pixel_width) + (x + i)] = cxy;
		}
	}
}

static void	interpolate_block(t_tile *tile, int x, int y)
{
	t_coef	coef;
	int		i;
	int		j;
	int		width;
	float	tx;
	float	ty;
	t_vec3f	cx0;
	t_vec3f	cx1;
	t_vec3f	cxy;
	int		index;

	init_coef(&coef, tile, x, y);
	width = tile->pixel_width;
	j = 0;
	while (j < 3)
	{
		i = 0;
		while (i < 3)
		{
			tx = (float)i / 3.0f;
			ty = (float)j / 3.0f;
			cx0 = vec3f_add(vec3f_scale(coef.c00, 1.0f - tx),
					vec3f_scale(coef.c01, tx));
			cx1 = vec3f_add(vec3f_scale(coef.c10, 1.0f - tx),
					vec3f_scale(coef.c11, tx));
			cxy = vec3f_add(vec3f_scale(cx0, 1.0f - ty),
					vec3f_scale(cx1, ty));
			index = (y + j) * width + (x + i);
			tile->hdr_pixels[index] = cxy;
			i++;
		}
		j++;
	}
}


static void	interpolate_tile_colors(t_tile *tile)
{
	int	local_y;
	int	local_x;

	local_y = 0;
	while (local_y + 3 < tile->pixel_height)
	{
		local_x = 0;
		while (local_x + 3 < tile->pixel_width)
		{
			interpolate_block(tile, local_x, local_y);
			local_x += 3;
		}
		local_y += 3;
	}
}

static void	replicate_right_edge(t_tile *tile)
{
	int	x_last;
	int	y;
	int	width;
	int	height;
	int	index_src;
	int	index_dst;

	width = tile->pixel_width;
	height = tile->pixel_height;
	if (width < 2)
		return ;
	x_last = width - 1;
	y = 0;
	while (y < height)
	{
		index_src = y * width + (x_last - 1);
		index_dst = y * width + x_last;
		tile->hdr_pixels[index_dst] = tile->hdr_pixels[index_src];
		y++;
	}
}

static void	replicate_bottom_edge(t_tile *tile)
{
	int	y_last;
	int	x;
	int	width;
	int	height;
	int	index_src;
	int	index_dst;

	width = tile->pixel_width;
	height = tile->pixel_height;
	if (height < 2)
		return ;
	y_last = height - 1;
	x = 0;
	while (x < width)
	{
		index_src = (y_last - 1) * width + x;
		index_dst = y_last * width + x;
		tile->hdr_pixels[index_dst] = tile->hdr_pixels[index_src];
		x++;
	}
}

static void	replicate_edges(t_tile *tile)
{
	replicate_right_edge(tile);
	replicate_bottom_edge(tile);
}

int	render_tile_fast(t_data *data, t_tile *tile, const t_render_view *view)
{
	t_vec3f	color;
	int		local_y;
	int		local_x;
	int		img_x;
	int		img_y;
	int		index;
	int		width;

	width = tile->pixel_width;
	local_y = 0;
	while (local_y < tile->pixel_height)
	{
		local_x = 0;
		while (local_x < tile->pixel_width)
		{
			img_x = tile->pixel_x + local_x;
			img_y = tile->pixel_y + local_y;
			calcul_pixel_color_fast(data, view, img_x, img_y, &color);
			index = local_y * width + local_x;
			tile->hdr_pixels[index] = color;
			local_x += 3;
		}
		local_y += 3;
	}
	interpolate_tile_colors(tile);
	replicate_edges(tile);
	return (SUCCESS);
}
