/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah_bins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:47:49 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 03:17:35 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "bvh.h"
#include "float.h"

__attribute__((always_inline))
extern inline float	prim_centroid_axis(const t_primref *p, int axis)
{
	if (axis == 0)
		return (p->centroid.x);
	if (axis == 1)
		return (p->centroid.y);
	return (p->centroid.z);
}

__attribute__((always_inline))
extern inline void	zero_bins(t_bin *bins, size_t siz)
{
	size_t	i;

	i = 0;
	while (i < siz)
	{
		bins[i].count = 0;
		bins[i].bounds.b[0] = (t_vec3f){FLT_MAX, FLT_MAX, FLT_MAX, 0.0f};
		bins[i].bounds.b[1] = (t_vec3f){-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f};
		i += 1;
	}
}

__attribute__((always_inline))
extern inline void	instance_cbounds(t_bvh_buf *b, t_bvh_child e,
	t_vec3f *cbounds)
{
	t_vec3f	tmp_c;
	int		i;

	cbounds[0] = b->pref[b->pref_idx[e.start]].centroid;
	cbounds[1] = cbounds[0];
	i = 1;
	while (i < e.count)
	{
		tmp_c = b->pref[b->pref_idx[e.start + i]].centroid;
		cbounds[0] = vec3f_min(cbounds[0], tmp_c);
		cbounds[1] = vec3f_max(cbounds[1], tmp_c);
		i += 1;
	}
}

__attribute__((always_inline))
extern inline int	fill_bins(t_bvh_buf *b, t_bvh_child e, t_vec3f *cbounds,
		int axis)
{
	int		i;
	float	tmp_c;
	int		bidx;
	t_bin	*bin;

	if (cbounds[1][axis] - cbounds[0][axis] <= EPSILON)
		return (1);
	i = 0;
	while (i < e.count)
	{
		tmp_c = prim_centroid_axis(&b->pref[b->pref_idx[e.start + i]], axis);
		bidx = (int)(((tmp_c - cbounds[0][axis])
					/ (cbounds[1][axis] - cbounds[0][axis]))
				* (float)BIN_COUNT);
		if (bidx == BIN_COUNT)
			bidx = BIN_COUNT - 1;
		bin = &b->bins[bidx];
		bin->bounds = bound_merge(bin->bounds,
				b->pref[b->pref_idx[e.start + i]].bounds);
		bin->count += 1;
		i += 1;
	}
	return (0);
}
