/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tonemap_hist_core.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:14:27 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:14:40 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tonemap_internal.h"
#include <math.h>

float	tm_luminance(const t_vec3f *color_lin)
{
	float	lum;

	lum = 0.2126f * (*color_lin)[0]
		+ 0.7152f * (*color_lin)[1]
		+ 0.0722f * (*color_lin)[2];
	if (lum < 0.0f)
		lum = 0.0f;
	return (lum);
}

void	tm_hist_clear(float *hist)
{
	int	index;

	index = 0;
	while (index < TONESIZE)
	{
		hist[index] = 0.0f;
		index++;
	}
}

int	tm_bin_from_lum(float lum, float log_min, float inv_log_range)
{
	float	x;
	int		bin;

	if (lum <= TM_LUM_MIN)
		return (0);
	if (lum >= TM_LUM_MAX)
		return (TONESIZE - 1);
	x = (logf(lum) - log_min) * inv_log_range;
	bin = (int)(x * (float)TONESIZE);
	if (bin < 0)
		bin = 0;
	if (bin >= TONESIZE)
		bin = TONESIZE - 1;
	return (bin);
}

float	tm_hist_total(const float *hist)
{
	int		index;
	float	total;

	index = 0;
	total = 0.0f;
	while (index < TONESIZE)
	{
		total += hist[index];
		index++;
	}
	return (total);
}

float	tm_lum_from_bin(int bin, float lum_min, float lum_max)
{
	float	t;
	float	ratio;

	t = ((float)bin + 0.5f) / (float)TONESIZE;
	ratio = lum_max / lum_min;
	return (lum_min * powf(ratio, t));
}
