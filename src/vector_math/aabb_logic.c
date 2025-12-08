/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb_logic.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: norivier <norivier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 02:01:43 by norivier          #+#    #+#             */
/*   Updated: 2025/10/31 02:10:30 by norivier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_math.h"
#include "types.h"
#include "math.h"
#include "bvh.h"
#include "vector.h"

FORCEINLINE
extern inline t_aabb	bound_merge(const t_aabb a, const t_aabb b)
{
	t_aabb	out;

	out.b[0] = vec3f_min(a.b[0], b.b[0]);
	out.b[1] = vec3f_max(a.b[1], b.b[1]);
	return (out);
}

FORCEINLINE
extern inline t_vec3f	aabb_centroid(const t_aabb a)
{
	return ((a.b[0] + a.b[1]) * RCP_2);
}

FORCEINLINE
extern inline float	bound_area(t_aabb b)
{
	t_vec3f	d;

	d.x = b.b[1].x - b.b[0].x;
	d.y = b.b[1].y - b.b[0].y;
	d.z = b.b[1].z - b.b[0].z;
	return (2.0f * (d.x * d.y + d.y * d.z + d.z * d.x));
}

FORCEINLINE
extern inline void	prim_bound_init(t_primref *pref, t_primitive *prims, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pref[i].prim_id = i;
		pref[i].bounds = prim_bound(&prims[i]);
		pref[i].centroid = aabb_centroid(pref[i].bounds);
		i += 1;
	}
}

FORCEINLINE
extern inline int	bound_intersect(t_ray r, t_aabb bound, float *near,
	float *far)
{
	t_aabb_inter	b;
	float			tmin;
	float			tmax;

	b.tminx = bound.b[r.sign[0]].x * r.invdir.x - r.orig_div.x;
	b.tmaxx = bound.b[1 - r.sign[0]].x * r.invdir.x - r.orig_div.x;
	b.tminy = bound.b[r.sign[1]].y * r.invdir.y - r.orig_div.y;
	b.tmaxy = bound.b[1 - r.sign[1]].y * r.invdir.y - r.orig_div.y;
	b.tminz = bound.b[r.sign[2]].z * r.invdir.z - r.orig_div.z;
	b.tmaxz = bound.b[1 - r.sign[2]].z * r.invdir.z - r.orig_div.z;
	tmin = fmaxf(fmaxf(b.tminx, b.tminy), b.tminz);
	tmax = fminf(fminf(b.tmaxx, b.tmaxy), b.tmaxz);
	if (tmax < fmaxf(tmin, 0.0f))
		return (0);
	if (near != ((void *)0))
		near[0] = tmin;
	if (far != ((void *)0))
		far[0] = tmax;
	return (1);
}
