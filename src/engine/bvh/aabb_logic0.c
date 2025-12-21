/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_logic0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 01:05:44 by norivier          #+#    #+#             */
/*   Updated: 2025/12/21 03:15:34 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvh.h"

__attribute__((always_inline))
t_aabb	prim_bound_range(t_primref *pref, int *indice, int start, int count)
{
	t_aabb	out;
	int		i;

	out = pref[indice[start]].bounds;
	i = 1;
	while (i < count)
	{
		out = bound_merge(out, pref[indice[start + i]].bounds);
		i += 1;
	}
	return (out);
}
