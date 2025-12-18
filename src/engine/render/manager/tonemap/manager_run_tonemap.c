/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager_run_tonemap.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:15:48 by kearmand          #+#    #+#             */
/*   Updated: 2025/12/17 17:35:31 by kearmand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "tonemap_internal.h"

#define QUALITY_FAST 1

int	manager_run_tonemap(t_mgr *mgr, int width, int height)
{
	float		lum95;
	float		exposure;
	t_tileset	*tileset;

	tileset = &mgr->tileset;
	if (mgr->render_view.mode == QUALITY_FAST
		|| !(tileset->tiles_done >= tileset->tiles_total))
		return (0);
	tm_build_hist_sampled(mgr, width, height);
	lum95 = tm_percentile_lum(mgr->tonemap_exposure, TM_PERCENTILE);
	exposure = tm_exposure_from_lum(lum95, TM_KEY);
	tm_apply_full_image(mgr, width, height, exposure);
	return (1);
}
