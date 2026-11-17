/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_run_tonemap.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:15:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/19 16:22:05 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "tonemap_internal.h"

float	manager_auto_exposure(t_mgr *mgr, int width, int height)
{
	float	lum95;

	tm_build_hist_sampled(mgr, width, height);
	lum95 = tm_percentile_lum(mgr->tonemap_exposure, TM_PERCENTILE);
	return (tm_exposure_from_lum(lum95, TM_KEY));
}
