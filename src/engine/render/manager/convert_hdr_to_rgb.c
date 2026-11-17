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
#include "display.h"
#include "tonemap/tonemap_internal.h"
#include <math.h>

/* Quadratic-domain LUT: finer sampling in the dark range. */
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
	pos = sqrtf(x) * (float)(POST_LUT_SIZE - 1);
	i = (int)pos;
	if (i >= POST_LUT_SIZE - 1)
		return (255);
	value = lut[i] + (lut[i + 1] - lut[i]) * (pos - (float)i);
	return ((int)(value + 0.5f));
}

static t_vec3f	map_color(t_vec3f color, float exposure, int enabled)
{
	float	lum;
	float	scale;

	if (!enabled)
		return (color * exposure);
	lum = tm_luminance(&color);
	scale = 0.0f;
	if (lum > TM_EPS)
		scale = tm_filmic_exp(lum, exposure) / lum;
	return (color * scale);
}

void	manager_convert_image(t_mgr *mgr, const t_vec3f *pixels,
		int *dst, size_t count, float auto_exposure, int quality)
{
	float	exposure;
	t_vec3f	color;
	int		gamma_step;
	int		enabled;
	size_t	i;

	gamma_step = post_gamma(mgr->post_settings);
	if (mgr->lut_gamma != gamma_step)
		build_gamma_lut(mgr, gamma_step);
	enabled = (mgr->post_settings & 1u) && quality != QUALITY_FAST;
	exposure = exp2f((float)post_ev(mgr->post_settings) * 0.1f);
	if (enabled && (mgr->post_settings & 2u))
		exposure *= auto_exposure;
	i = 0;
	while (i < count)
	{
		color = map_color(pixels[i], exposure, enabled);
		dst[i] = (encode_channel(mgr->gamma_lut, color[0]) << 16)
			| (encode_channel(mgr->gamma_lut, color[1]) << 8)
			| encode_channel(mgr->gamma_lut, color[2]);
		i++;
	}
}
