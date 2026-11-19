/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_hdr_to_rgb.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 18:18:51 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 23:06:12 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager.h"
#include "tonemap/tonemap_internal.h"
#include <math.h>

/* Quartic-domain LUT retains precision near black at high gamma. */
static void	build_gamma_lut(t_mgr *mgr, int gamma_step)
{
	int		i;
	float	x;
	float	y;
	float	inv_gamma;

	inv_gamma = 20.0f / (float)gamma_step;
	i = 0;
	while (i < POST_LUT_SIZE)
	{
		x = (float)i / (float)(POST_LUT_SIZE - 1);
		x *= x;
		y = powf(x * x, inv_gamma);
		if (y <= 0.0031308f)
			y *= 12.92f;
		else
			y = 1.055f * powf(y, 1.0f / 2.4f) - 0.055f;
		mgr->gamma_lut[i++] = y * 255.0f;
	}
	mgr->lut_gamma = gamma_step;
}

static int	encode_channel(const float *lut, float x)
{
	float	pos;
	float	value;
	int		i;

	if (!(x > 0.0f))
		return (0);
	if (x >= 1.0f)
		return (255);
	pos = sqrtf(sqrtf(x)) * (float)(POST_LUT_SIZE - 1);
	i = (int)pos;
	if (i >= POST_LUT_SIZE - 1)
		return (255);
	value = lut[i] + (lut[i + 1] - lut[i]) * (pos - (float)i);
	return ((int)(value + 0.5f));
}

static t_vec3f	map_color(t_vec3f color, float exposure)
{
	float	lum;
	float	scale;

	lum = tm_luminance(&color);
	scale = 0.0f;
	if (lum > 0.0f)
		scale = tm_filmic_exp(lum, exposure) / lum;
	return (color * scale);
}

static int	pack_color(t_mgr *mgr, t_vec3f hdr, float exposure)
{
	t_vec3f	color;

	color = map_color(hdr, exposure);
	return ((encode_channel(mgr->gamma_lut, color[0]) << 16)
		| (encode_channel(mgr->gamma_lut, color[1]) << 8)
		| encode_channel(mgr->gamma_lut, color[2]));
}

/* A flat run (sky, an unlit surface, or an untouched tile) needs just
 * one filmic mapping. Compare linear RGB exactly, before quantization. */
static void	convert_row(t_mgr *mgr, const t_vec3f *src, int *dst,
				int width, float exposure)
{
	t_vec3f	sample;
	int		rgb;
	int		i;

	i = 0;
	while (i < width)
	{
		sample = src[i];
		rgb = pack_color(mgr, sample, exposure);
		dst[i++] = rgb;
		while (i < width && sample[0] == src[i][0]
			&& sample[1] == src[i][1] && sample[2] == src[i][2])
			dst[i++] = rgb;
	}
}

static void	convert_tile(t_render *r, t_rgb_image *image, int id, float exposure)
{
	t_mgr			*mgr;
	const t_vec3f	*hdr;
	int				x;
	int				y;
	int				width;
	int				end_y;
	int				i;

	mgr = &r->manager;
	hdr = mgr->frames[image->camera_id].pixels;
	x = id % mgr->tileset.tiles_per_row * TILE_SIZE;
	y = id / mgr->tileset.tiles_per_row * TILE_SIZE;
	width = r->width - x;
	if (width > TILE_SIZE)
		width = TILE_SIZE;
	end_y = y + TILE_SIZE;
	while (y < end_y && y < r->height)
	{
		i = y * r->width + x;
		convert_row(mgr, hdr + i, image->pixels + i, width, exposure);
		y++;
	}
}

/* Yield to dispatch after four changed tiles. Unchanged tiles cost only
 * a sequence comparison. A running pass keeps its original settings. */
int	manager_convert_tiles(t_render *r, t_rgb_image *image)
{
	t_mgr		*mgr;
	t_hdr_frame	*frame;
	float		exposure;
	int			budget;
	int			id;

	mgr = &r->manager;
	frame = &mgr->frames[image->camera_id];
	if (mgr->lut_gamma != post_gamma(image->settings))
		build_gamma_lut(mgr, post_gamma(image->settings));
	exposure = exp2f((float)post_ev(image->settings) * 0.1f);
	budget = 4;
	while (budget && mgr->pass.next_tile < mgr->tileset.tiles_total)
	{
		id = mgr->pass.next_tile++;
		if (image->tile_seq[id] != frame->tile_seq[id])
		{
			convert_tile(r, image, id, exposure);
			image->tile_seq[id] = frame->tile_seq[id];
			budget--;
		}
	}
	return (mgr->pass.next_tile == mgr->tileset.tiles_total);
}
