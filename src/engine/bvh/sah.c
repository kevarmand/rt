/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kearmand <kearmand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:43:46 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 01:40:21 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"
#include "attributes.h"
#include "float.h"

FORCEINLINE
static inline void	build_prefix_suffix(t_bvh_buf *b)
{
	int	i;

	b->prefix[0] = b->bins[0].bounds;
	i = 1;
	while (i < BIN_COUNT)
	{
		if (b->bins[i].count)
			b->prefix[i] = bound_merge(b->prefix[i - 1], b->bins[i].bounds);
		else
			b->prefix[i] = b->prefix[i - 1];
		i += 1;
	}
	b->suffix[BIN_COUNT - 1] = b->bins[BIN_COUNT - 1].bounds;
	i = BIN_COUNT - 2;
	while (i >= 0)
	{
		if (b->bins[i].count)
			b->suffix[i] = bound_merge(b->suffix[i + 1], b->bins[i].bounds);
		else
			b->suffix[i] = b->suffix[i + 1];
		i -= 1;
	}
}

FORCEINLINE
static inline void	prefix_suffix_count(t_bvh_buf *b)
{
	int	i;
	int	sum;

	i = 0;
	sum = 0;
	while (i < BIN_COUNT)
	{
		sum += b->bins[i].count;
		b->left_counts[i] = sum;
		i += 1;
	}
}

FORCEINLINE
static inline void	find_bestcost(t_bvh_buf *b, t_bvh_child e, int axis)
{
	int		i;
	int		counts[2];
	float	cost;

	i = -1;
	while (i < BIN_COUNT - 1)
	{
		i += 1;
		counts[0] = b->left_counts[i];
		counts[1] = e.count - counts[0];
		if (counts[0] == 0 || counts[1] == 0)
			continue ;
		cost = bound_area(b->prefix[i]) * (float)counts[0]
			+ bound_area(b->suffix[i + 1]) * (float)counts[1];
		if (cost < b->bestcost)
		{
			b->bestcost = cost;
			b->bestaxis = axis;
			b->bin_split_idx = i;
		}
	}
}

FORCEINLINE
static inline int	partition_inplace(t_bvh_buf *b, t_bvh_child e,
	t_vec3f *cbounds)
{
	int		low;
	int		high;
	float	cent;
	int		bidx;

	low = e.start;
	high = e.start + e.count - 1;
	while (low <= high)
	{
		cent = prim_centroid_axis(&b->pref[b->pref_idx[low]], b->bestaxis);
		bidx = (int)(((cent - cbounds[0][b->bestaxis])
					/ (cbounds[1][b->bestaxis]
						- cbounds[0][b->bestaxis])) * BIN_COUNT);
		if (bidx >= BIN_COUNT)
			bidx = BIN_COUNT - 1;
		if (bidx > b->bin_split_idx)
		{
			swap_int(&b->pref_idx[low], &b->pref_idx[high]);
			high -= 1;
		}
		else
			low += 1;
	}
	return (low - e.start);
}

int	sah_split(t_bvh_child e, t_bvh_buf *b)
{
	t_vec3f	cbounds[2];
	int		split_idx;
	int		axis;

	if (e.count < 2)
		return (-1);
	b->bestcost = FLT_MAX;
	b->bestaxis = -1;
	b->bin_split_idx = -1;
	instance_cbounds(b, e, cbounds);
	axis = -1;
	while (axis < 3)
	{
		axis += 1;
		zero_bins(b->bins, BIN_COUNT);
		if (fill_bins(b, e, cbounds, axis) != 0)
			continue ;
		build_prefix_suffix(b);
		prefix_suffix_count(b);
		find_bestcost(b, e, axis);
	}
	if (b->bestaxis < 0)
		return (-1);
	split_idx = partition_inplace(b, e, cbounds);
	return (split_idx);
}
