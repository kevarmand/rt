/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tonemap_hist_build.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:15:02 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:36:13 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tonemap_internal.h"
#include <math.h>

float	tm_percentile_lum(const float *hist, float percentile)
{
	float	total;
	float	target;
	float	acc;
	int		bin;

	total = tm_hist_total(hist);
	target = total * percentile;
	acc = 0.0f;
	bin = 0;
	while (bin < TONESIZE)
	{
		acc += hist[bin];
		if (acc >= target)
			return (tm_lum_from_bin(bin, TM_LUM_MIN, TM_LUM_MAX));
		bin++;
	}
	return (TM_LUM_MAX);
}

void	tm_build_hist_sampled(t_mgr *mgr, int width, int height)
{
	int		x;
	int		y;
	float	lum;
	float	log_min;
	float	inv_log_range;

	log_min = logf(TM_LUM_MIN);
	inv_log_range = 1.0f / (logf(TM_LUM_MAX) - log_min);
	tm_hist_clear(mgr->tonemap_exposure);
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			if (((x + y) & 1) == 0)
			{
				lum = tm_luminance(&mgr->hdr_buffer[y * width + x]);
				mgr->tonemap_exposure[tm_bin_from_lum(lum, log_min,
						inv_log_range)] += 1.0f;
			}
			x++;
		}
		y++;
	}
}
